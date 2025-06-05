// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtScenarioControllerBase.h"

#include <Delegates/DelegateCombinations.h>

#include "Common/RtCommon.h"
#include "Common/RtWebCommon.h"

#include "RtGameScenarioController.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(RtGameScenarioController, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSctShowMessage, FString, message, float, displayTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSctGamePlay, bool, IsGamePlay);

#define ALL_NPC_UPDATE_POS_TIMER_TIME 1.0f

/**
 *
 */

class ARtWidgetActor;
class ARtWidgetActor_Menu;
class URt_Trainee_Text;
class URt_Trainee_SelectionMenu;
class ARtObjectActor;
class ARtReplayManagerActor;
class USphereComponent;
class ARtTraineePawn;
class ARtPlayerState;
class ARtGameState;
class ARtPlayerController;

struct FShotData
{
	FShotData(FString wID, FString hitOID, FVector hitP, FVector shotP, FVector shotDir, bool amm)
		:WeaponID(wID), HitObjectID(hitOID), HitPoint(hitP), ShotPoint(shotP), ShotDirection(shotDir), Ammunition(amm)
	{};
	FString WeaponID = "";
	FString HitObjectID = "";
	FVector HitPoint = FVector::ZeroVector;
	FVector ShotPoint = FVector::ZeroVector;
	FVector ShotDirection = FVector::ZeroVector;
	bool Ammunition = false;
};

UCLASS()
class RTGAME_API ARtGameScenarioController : public ARtScenarioControllerBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	DECLARE_DELEGATE_TwoParams(FDelegateCallCommand, TArray<FRtSctFunctionCommandParam>, bool&);
	DECLARE_DELEGATE(FDelegateChainAniEnd);

	TMap<ERtSctFunCmd, FDelegateCallCommand> CallCommandFunctionMap;

	TMap<FString, ERtSctGameState> StringToESctGameStateMap;

	FString CurrentPlayerID = TEXT("");

	bool bIsGamePlay = false;

	FRtScenarioEventCommand* CurrentSctEventCommand = nullptr;

	class ARtGameNetworkHandler* NetworkHandler = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "ARtMct2ScenarioController")
	FSctShowMessage SctShowMessage;

	ERt_PlayType CurrentPlayType = ERt_PlayType::E_None;
	float AllNpcPosTimer = 0;

	FString SctFileName; // �ε��ϴ� Sct ���� �̸�.

	FString RifleAimedObjectKeyName; // ������ �ܴ� �� Ȯ�εǴ� ��ü�� Ű �̸�.
	FString RifleShotObjectKeyName; // ������ �� ���� ��ü�� Ű �̸�.
	FVector RifleAimedHitLocation; // ������ �ܴ� ��ǥ
	TWeakObjectPtr<ARtObjectActor> AimedTarget; // ������ �ܴ� ��ü ����

	float TrainingTimer = -1;

	void SetRifleTargetObjectKeyName(bool bIsAimed, FString objectKeyName, FVector hitLocation);

	void Init(ERt_PlayType currentPlayType, FString sctFileName);
	void InitScenario();

	//////////////////////////////////////////////////////////////////////////
	// Sct ��ũ��Ʈ �Լ� ����Ʈ.
	//////////////////////////////////////////////////////////////////////////

	void IsGameState(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsArriveGameReadyPlace(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void JustDoIt(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsOldValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsVisible(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsAnimProgress(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsTargetInteract(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsInRadius(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsTargetAimed(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsHitObject(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsIntimidatingFire(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsTrainingElapsedAt(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void CheckCollision(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void ReqGameEnd(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void NotDoing(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void SetGameState(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void SetEnableGameReadyPlace(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void SetValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void DelayTime(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void SetVisible(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void SetVisibleGroup(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void PlayAnim(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void MoveToWayPoint(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void NpcMoveToWayPoint(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void StopAct(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void AttachWeapon(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void AttackTarget(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void NpcInit(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void ShowScreenText(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void ShowTargetText(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void ShowScreenMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void ShowTargetMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void RemoveScreenText(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void RemoveTargetText(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void RemoveMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void RecordResultLog(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void PlayMovie(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void SetTargetMenuEnabled(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void SendLog(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void IsNotValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsGreaterEqualValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void IsLessEqualValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	void SetVisibleRadioMenuItem(TArray<FRtSctFunctionCommandParam> param, bool& retValue);
	void SendPacketToScenarioServerForEnd(int32 TID);
	void EndTraining(TArray<FRtSctFunctionCommandParam> param, bool& retValue);

	bool Force_RequestRandomNum(const TArray<FRtSctFunctionCommand>* commandArray);

	//////////////////////////////////////////////////////////////////////////

	TArray<FString> GetObjectIDList(FString objectID);

	FRtScenarioEvent* GetScenarioEvent(int32 index);

	FRtScenarioPlayerGroup* GetScenarioPlayerGroupInfo(FString playerObjectID);

	FRtScenarioPlayer* GetScenarioPlayerInfo(FString playerObjectID);

	FString GetModelPathByObjectID(FString playerObjectID);

	bool IsTctType(FString modleKeyName, ERtTctType inputTctType);

	bool AttachWeapon(FString characterId, FString equipmentId);

public:
	ARtGameScenarioController();

	void Bind_DelegateCommands();
	void Regist_ConsoleCommands();

	virtual void Tick(float DeltaTime) override;

	bool ProcessOneEvent(FRtScenarioEventCommand* scenarioEventCommand);

	UFUNCTION(BlueprintCallable)
	void SetCurrentPlayerID(FString playerID);

	UFUNCTION(BlueprintCallable)
	void SetGamePlay(bool isGamePlay);

	FSctGamePlay SetSctGamePlay;

	UFUNCTION(BlueprintCallable)
	void LogOut();

	class URtNetworkManager* Get_NetManager();
	class URtWebManager* Get_WebManager();

	void ExecuteServerEvent(const FString& eventID); // �������� �߻��� �̺�Ʈ�� �޾Ƽ� ó��

	void LineTraceAim();
	void Interact_AimTarget();
	void Open_DefaultMenu(const FVector& loc, const FRotator& rot);
	void Interact_Target(TWeakObjectPtr<ARtObjectActor> target, USphereComponent* handCollision = nullptr);
	void Interact_TargetEvent();
	bool OpenDropbox(TWeakObjectPtr<ARtObjectActor>& target, FVector Location);
	void CloseMenu();
	bool HideMenuWidget(bool IsImmediate = false);
	//void ExecuteMenuOption(class ARtWidgetActor_MenuOption* MenuOption);

	void UpdateListeningNpc(const FString& CurNpcName);
	void ExecuteCommandToNpc(const FString& targetNpcName, const FString& commandName);

	void AddAimMap(const int32& playerID, const FString& objectID);
	void AddShotList(const FString& weaponID, const FString& objectID, const FVector& hitPosition, const FVector& shotPosition, const FVector& shotDirection, const bool& amm);
	void AddInteractKeyList(const int32& playerID);

	void ChangeSelectionMenu(const float& value); // �޴��� �ڵ���Ʈ�ѷ� ����Ǿ� ���
	void SelectMenu();

	void UpdateNpcState(struct stNpcState& npcState);
	bool ExecuteProcessCommands(const FRtEventCommand* pEvent, bool isReceive = false);

	//HACK: ����� �׽�Ʈ��. ���� ��Ƽ ����ؾ� ��.
	UFUNCTION(BlueprintCallable)
	void Surrender(FString npcID);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* AimLine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineMeshComponent* AimLineMesh;

	bool IsTargetMenuEnable(FString targetMenuName);
	TMap<FString, class ARtGuideHand*> GuideHandMap; // Ÿ�ٸ޴�ID, GuideHand
	TMap<FString, class ARtWidgetActor_ListenIcon*> ListenIconMap; // Ÿ�ٸ޴�ID, ListenIcon

	ARtWidgetActor_ListenIcon* ActivatedListenIconActor = nullptr;

	ARtPlayerController* GetMyPlayerController(); // �� �÷��̾� ��Ʈ�ѷ��� ã�´�.
	ARtTraineePawn* GetMyTraineePawn(); // �� �Ʒ��� ���� ã�´�.
	ARtPlayerState* GetMyPlayerState(); // �� �÷��̾� ������Ʈ�� ã�´�.
	ARtGameState* GetGameState(); // ���� ������Ʈ�� ã�´�.

protected:
	void MoveToWayPoint_Progress();
	TMap<ARtObjectActor*, TPair<ARtObjectActor*, float>> MoveToWayPointMap; //MoveActor, WayPoints, MoveSpeed

	bool CreateTargetTextWidgetActor(ARtWidgetActor*& widgetActor, URt_Trainee_Text*& widget, FString text, AActor* target, FString& errorMsg);
	bool CreateMenuWidgetActor(ARtWidgetActor*& widgetActor, URt_Trainee_SelectionMenu*& widget, FString menuSetID, FString& errorMsg);

	void RemoveTargetTextWidgetActor(FString text, AActor* target);
	void RemoveMenuWidgetActor(FString menuSetID);

	TMap<TPair<FString, AActor*>, ARtWidgetActor*> TargetTextWidgetMap;

	UPROPERTY()
	TWeakObjectPtr<ARtWidgetActor_Menu> MenuWidget = nullptr;

	TMap<int32, FString> Resv_AimMap; // PlayerUID, ObjectID
	TMap<int32, FString> Process_NewAimMap; // PlayerUID, ObjectID
	TMap<int32, FString> Process_AimingMap; // PlayerUID, ObjectID
	bool IsChangeAim = false;

	TArray<int32> Resv_InteractKeyList; // PlayerUID
	TArray<int32> Process_InteractKeyList; // PlayerUID

	TArray<FShotData> Resv_ShotList;
	TArray<FShotData> Process_ShotList;

	TMap<FString, TPair<int, int>> NewVarialbleMap; // ������, OldValue, NewValue

	TSet<FString> DisabledTargetMenuList; // Ÿ�� �޴��� ��Ȱ��ȭ �� ������ƮID

	bool ConditionLog = false;
	bool DebugMode = false;
	bool DebugValue = false;
	bool DebugWayPoint = false;
	bool DebugRadius = false;
	bool DebugCollision = false;
	bool DebugIntimidatingFire = false;
	bool DebugHit = false;

	TWeakObjectPtr<class URt_Trainee_Main> MainWidget;
	class URt_Trainee_Main* GetMainWidget();
	class ARtWidgetActor_Radio* GetRadioWidget();
	class ARtWidgetActor_Menu* GetTargetMenuWidget();

	bool IsGuideAim = false;
	float IntimidatingFire_Distance = 1000.0f; // ������� �Ÿ�(cm)
	float IntimidatingFire_Angle = 45.0f; // ������� ����(��)

	TMap<FString, int> DebugCondition;

	TMap<ERtSctSendLogType, ELogProtocol> SendLogMap; // �α� ������ ��ȣ ���� ( Sct �α� �� -> �α� ��ȣ �� )
	
public:
	static ARtGameScenarioController* Get(UObject* worldContext) { return Cast<ARtGameScenarioController>(ARtScenarioControllerBase::Get(worldContext)); }

protected:
	bool IsInRange(AActor* startActor, const FVector& startLoc, const FVector& startDir, AActor* target, const float& maxDistance, const float& maxAngle);

};
