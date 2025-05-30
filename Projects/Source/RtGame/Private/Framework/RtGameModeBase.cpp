// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtGameModeBase.h"
// Unreal Basic
#include "Kismet/GameplayStatics.h"

// Framework
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"


DEFINE_LOG_CATEGORY(LogRtServer);

ARtGameModeBase::ARtGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// initialize value
    InactivePlayerStateLifeSpan = 300.f;   // 로그아웃 이후 비활성화된 플레이어 스테이트가 머무르는 시간
	//bDelayed_Start = false;//
	//Min_RespawnDelay = 1.0f;
	Max_InactivePlayers = 16;                
}

void ARtGameModeBase::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	check(PlayerState)
		UWorld* LocalWorld = GetWorld();
	// don't store if it's an old PlayerState from the previous level or if it's a spectator... or if we are shutting down
	if (!PlayerState->IsFromPreviousLevel() && !MustSpectate(PC) && !LocalWorld->bIsTearingDown)
	{
		APlayerState* const NewPlayerState = PlayerState->Duplicate();
		if (NewPlayerState)
		{
			// Side effect of Duplicate() adding PlayerState to PlayerArray (see APlayerState::PostInitializeComponents)
			GameState->RemovePlayerState(NewPlayerState);

			// make PlayerState inactive
			NewPlayerState->SetReplicates(false);

			// delete after some time
			NewPlayerState->SetLifeSpan(InactivePlayerStateLifeSpan);

			// On console, we have to check the unique net id as network address isn't valid
			const bool bIsConsole = !PLATFORM_DESKTOP;
			// Assume valid unique ids means comparison should be via this method
			const bool bHasValidUniqueId = NewPlayerState->GetUniqueId().IsValid();
			// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
			const bool bHasValidNetworkAddress = !NewPlayerState->SavedNetworkAddress.IsEmpty();
			const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;

			// make sure no duplicates, 추가 전 확실하게 지움
			for (int32 Idx = 0; Idx < InactivePlayerArray.Num(); ++Idx)
			{
				APlayerState* const CurrentPlayerState = InactivePlayerArray[Idx];
				if ((CurrentPlayerState == nullptr) || IsValid(CurrentPlayerState))
				{
					// already destroyed, just remove it
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
				else if ((!bUseUniqueIdCheck && bHasValidNetworkAddress && (CurrentPlayerState->SavedNetworkAddress == NewPlayerState->SavedNetworkAddress))
					|| (bUseUniqueIdCheck && (CurrentPlayerState->GetUniqueId() == NewPlayerState->GetUniqueId())))
				{
					// destroy the playerstate, then remove it from the tracking
					CurrentPlayerState->Destroy();
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
			}
			InactivePlayerArray.Add(NewPlayerState);

			// make sure we dont go over the maximum number of inactive players allowed
			if (InactivePlayerArray.Num() > Max_InactivePlayers)
			{
				int32 const NumToRemove = InactivePlayerArray.Num() - Max_InactivePlayers;

				// destroy the extra inactive players
				for (int Idx = 0; Idx < NumToRemove; ++Idx)
				{
					APlayerState* const PS = InactivePlayerArray[Idx];
					if (PS != nullptr)
					{
						PS->Destroy();
					}
				}

				// and then remove them from the tracking array
				InactivePlayerArray.RemoveAt(0, NumToRemove);
			}
		}
	}
}



bool ARtGameModeBase::FindInactivePlayer(APlayerController* PC)
{
	check(PC && PC->PlayerState);
	// don't bother for spectators
	if (MustSpectate(PC))
	{
		return false;
	}

	// On console, we have to check the unique net id as network address isn't valid
	const bool bIsConsole = !PLATFORM_DESKTOP;
	// Assume valid unique ids means comparison should be via this method
	const bool bHasValidUniqueId = PC->PlayerState->GetUniqueId().IsValid();
	// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
	const bool bHasValidNetworkAddress = !PC->PlayerState->SavedNetworkAddress.IsEmpty();
	const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;

	const FString NewNetworkAddress = PC->PlayerState->SavedNetworkAddress;
	const FString NewName = PC->PlayerState->GetPlayerName();
	for (int32 i = 0; i < InactivePlayerArray.Num(); i++)
	{
		APlayerState* CurrentPlayerState = InactivePlayerArray[i];
		if ((CurrentPlayerState == nullptr) || !IsValid(CurrentPlayerState))
		{
			InactivePlayerArray.RemoveAt(i, 1);
			i--;
		}
		else if ((bUseUniqueIdCheck && (CurrentPlayerState->GetUniqueId() == PC->PlayerState->GetUniqueId())) ||
			(!bUseUniqueIdCheck && bHasValidNetworkAddress && (FCString::Stricmp(*CurrentPlayerState->SavedNetworkAddress, *NewNetworkAddress) == 0) && (FCString::Stricmp(*CurrentPlayerState->GetPlayerName(), *NewName) == 0)))
		{
			// found it!
			APlayerState* OldPlayerState = PC->PlayerState;
			PC->PlayerState = CurrentPlayerState;
			PC->PlayerState->SetOwner(PC);
			PC->PlayerState->SetReplicates(true);
			PC->PlayerState->SetLifeSpan(0.0f);
			OverridePlayerState(PC, OldPlayerState);
			GameState->AddPlayerState(PC->PlayerState);
			InactivePlayerArray.RemoveAt(i, 1);
			OldPlayerState->SetIsInactive(true);
			// Set the uniqueId to nullptr so it will not kill the player's registration 
			// in UnregisterPlayerWithSession()
			OldPlayerState->SetUniqueId(nullptr);
			OldPlayerState->Destroy();
			PC->PlayerState->OnReactivated();
			return true;
		}
	}
	return false;
}

void ARtGameModeBase::OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState)
{
	PC->PlayerState->DispatchOverrideWith(OldPlayerState);
}

void ARtGameModeBase::Post_Login_Sub(APlayerController* NewPlayer)
{
	UWorld* World = GetWorld();

	// update player count
	if (MustSpectate(NewPlayer))
	{
		Num_Spectators++;
	}
	else if (World->IsInSeamlessTravel() || NewPlayer->HasClientLoadedCurrentWorld())
	{
		Num_Players++;
	}
	else
	{
		Num_TravellingPlayers++;
	}

	// save network address for re-associating with reconnecting player, after stripping out port number
	FString Address = NewPlayer->GetPlayerNetworkAddress();
	int32 pos = Address.Find(TEXT(":"), ESearchCase::CaseSensitive);
	NewPlayer->PlayerState->SavedNetworkAddress = (pos > 0) ? Address.Left(pos) : Address;

	// check if this player is reconnecting and already has PlayerState
	FindInactivePlayer(NewPlayer);
}

bool ARtGameModeBase::IsServerMode()
{
	auto mode = GetNetMode();
	if (mode == ENetMode::NM_DedicatedServer || mode == ENetMode::NM_ListenServer)
		return true;
	return false;
}

bool ARtGameModeBase::ReadyToStartMatch()
{
	// If bDelayed Start is set, wait for a manual match start
	if (bDelayed_Start)
	{
		return false;
	}

	// By default start when we have > 0 players
	if (Get_MatchState() == RtMatchState::WaitingToStart)
	{
		if (Num_Players + Num_Bots > 0)
		{
			return true;
		}
	}
	return false;
}
