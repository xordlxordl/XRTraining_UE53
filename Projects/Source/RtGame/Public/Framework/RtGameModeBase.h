// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RtGameModeBase.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtServer, Log, All);

/**
 * 
 */
UCLASS()
class RTGAME_API ARtGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
	
// Various

private:
    /** Time a playerstate will stick around in an inactive state after a player logout */
	float InactivePlayerStateLifeSpan;

	/** The maximum number of inactive players before we kick the oldest ones out */
	int32 Max_InactivePlayers;


protected:

	/** Whether the game should immediately start when the first player logs in. Affects the default behavior of ReadyToStartMatch */
	uint32 bDelayed_Start : 1;

	/** Current number of spectators. */
	int32 Num_Spectators;

	/** Current number of human players. */
	int32 Num_Players;

	/** number of non-human players (AI controlled but participating as a player). */
	int32 Num_Bots;

	/** Minimum time before player can respawn after dying. */
	float Min_RespawnDelay;

	/** Number of players that are still traveling from a previous map */
	int32 Num_TravellingPlayers;

	/** PlayerStates of players who have disconnected from the server (saved in case they reconnect) */
	UPROPERTY()
		TArray<class APlayerState*> InactivePlayerArray;

	/** What match state we are currently in */
	UPROPERTY(Transient)
	    FName Match_State;  // Training Step         

	UPROPERTY(Transient)
	    FName InProgress_State;


// Function 

public:

// Player
	/** Add PlayerState to the inactive list, remove from the active list */
	void AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC);

	/** Override PC's PlayerState with the values in OldPlayerState as part of the inactive player handling */
	void OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState);

	/** Attempt to find and associate an inactive PlayerState with entering PC.
		* @Returns true if a PlayerState was found and associated with PC. */
	bool FindInactivePlayer(APlayerController* PC);



// Login
	void Post_Login_Sub(APlayerController* NewPlayer);
	
	bool IsServerMode();
	
	bool ReadyToStartMatch();

	FName Get_MatchState() const { return Match_State; }


};
