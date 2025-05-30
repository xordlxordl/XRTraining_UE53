// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE 
#include "http.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"

// Data
#include "Common/RtRpcCommon.h"

#include "Rt_ServerWidget.generated.h"


// Framework
class ARtGameState;
class ARtPlayerState;

/**
* 검은 화면만 출력할 예정이라 ListView 와 관련된 Update Device 함수들은 바인딩 할 필요가 없음
* 변경) 해당 위젯 자체를 쓰지 않을 것. 추후에 서버 화면에 무언가를 출력해야 할 때를 대비 
* 이하의 델리게이트 및 바인드 함수 또한 다른 곳과 무관하며 해당 위젯만을 위한 것
* 
 * 
 */
UCLASS()
class RTGAME_API URt_ServerWidget : public URt_PageBase
{
	GENERATED_BODY()
// Various
public:
	// Web
	FHttpModule* Http;

	// DelegateHandle
	FDelegateHandle OnGameModeMatchStateSetHandle;

// Function
public :
	// Main Flow
	virtual bool Initialize() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Delegate Bind
	void Update_Player_List();
	void Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray);
	void OnGameModeMatchStateSet(FName MatchState);  // Match State Set Delegate

protected:
	void Handle_PlayerLogin(AGameModeBase* InGameMode, AController* InPlayer);
};
