// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
// Unreal Basic
#include "Net/UnrealNetwork.h"
#include "Kismet/Gameplaystatics.h"
//Data
#include "Common/RtRpcCommon.h"

#include "RtPlayerState.generated.h"

// Framework
class ARtPlayerController;

UENUM()
enum class ERtPlayerConnectionType : uint8
{
	Player = 0,
	LiveSpectator,
	ReplaySpectator,
	InactivePlayer
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBasicInfoChangedDelegate, ARtPlayerState*, InPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeviceInfoChangedDelegate, int32, PlayerId, const FRtDeviceInfo&, InDeviceInfo);

/**
 * 
 */
UCLASS()
class RTGAME_API ARtPlayerState : public APlayerState
{
	GENERATED_BODY()

// Various
	
public:
   // Main Flow
	ARtPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

   UPROPERTY(Replicated)
       ERt_PlayType Play_Type;

   UPROPERTY(Replicated)
       ERtPlayerConnectionType MyPlayerConnectionType;

   //로그인에 사용된 정보를 따로 보관한다.
   UPROPERTY(Replicated)
	   FString Login_PlayerName;
   UPROPERTY(Replicated)
	   FString Login_PlayerId;
   UPROPERTY(Replicated)
	   FString Login_PlayerPassword;
   UPROPERTY(ReplicatedUsing = OnRep_Login_DeviceName)
	   FString Login_DeviceName;

	UPROPERTY(Replicated)
		bool IsVR;

// Delegate
	DECLARE_MULTICAST_DELEGATE_OneParam(FUpdated, ARtPlayerState*);
	static FUpdated Updated_Delegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FOnPlayerBasicInfoChangedDelegate OnPlayerBasicInfoChanged;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FOnDeviceInfoChangedDelegate OnDeviceInfoChanged;

	FOnPlayerBasicInfoChangedDelegate& Get_OnPlayerBasicInfoChangedDelegate() {
		return OnPlayerBasicInfoChanged;
	}
	FOnDeviceInfoChangedDelegate& Get_OnDeviceInfoChangedDelegate() {
		return OnDeviceInfoChanged;
	}

	// UE Basic Delegate
	FSimpleMulticastDelegate OnChanged_DeviceInfo_Delegate;
	FSimpleMulticastDelegate OnChanged_TraineeInfo_Delegate;

private:
	UPROPERTY(ReplicatedUsing = OnRep_DeviceInfoChanged, VisibleAnywhere)
		FRtDeviceInfo DeviceInfo;

// Functions
public:
	// Main Flow
	/** Called by Controller when its PlayerState is initially replicated. */
	virtual void ClientInitialize(class AController* C) override;

    // Costom Get Set
	UFUNCTION(BlueprintCallable)
	const FRtDeviceInfo& Get_PlayerInfo(); 
	ARtPlayerController* Get_PlayerController();
	int32 Get_PlayerID() { return PlayerId; }

	void Set_PlayerInfo(const FRtDeviceInfo& InInfo);
	void Set_LoginInfo(const FString& InOptions, bool InServer);
	void Set_AddtionalInformation(const FRtUserInfo_PreLogin& InInfo);  // 로그인 정보 추가.

	UFUNCTION(Server, Unreliable)
	void Server_Change_ReadyState(const FRtDeviceInfo& Info);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Change_ReadyState(const FRtDeviceInfo& Info);

	
	// Delegate
	void CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate Delegate);

	//// RPC
	//UFUNCTION(Server, Reliable)
	//void Server_BindingDelegate_Implementation();

protected:
// Unreal : Copy properties which need to be saved in inactive PlayerState
//	virtual void CopyProperties(APlayerState* PlayerState) override;



// OnRep
	UFUNCTION()	void OnRep_DeviceInfoChanged(const FRtDeviceInfo& InInfo);

	UFUNCTION()	void OnRep_Login_DeviceName();

public:
	virtual void OnRep_PlayerName() override;

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

};
