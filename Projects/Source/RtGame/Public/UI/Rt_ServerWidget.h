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
* ���� ȭ�鸸 ����� �����̶� ListView �� ���õ� Update Device �Լ����� ���ε� �� �ʿ䰡 ����
* ����) �ش� ���� ��ü�� ���� ���� ��. ���Ŀ� ���� ȭ�鿡 ���𰡸� ����ؾ� �� ���� ��� 
* ������ ��������Ʈ �� ���ε� �Լ� ���� �ٸ� ���� �����ϸ� �ش� �������� ���� ��
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
