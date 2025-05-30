// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtGameScenarioController.h"
#include "RtDefines.h"
#include "RtBluePrintCommonUtil.h"
#include "Manager/RtGameSceneManager.h"
#include <Kismet/GameplayStatics.h>
#include "Camera/CameraComponent.h"

#include "Common/RtBlueprintAssetPath.h"
#include "RtCore/Public/Common/RtPacket.h"
#include "Manager/RtConfig.h"

#include "Framework/RtPlayerController.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtWidgetActor.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtGameState.h"
#include "Framework/RtWidgetActor.h"
#include "Framework/RtListenIconUserWidget.h"
#include "Framework/RtWidgetActor_ListenIcon.h"
#include "Framework/RtWidgetActor_Menu.h"
#include "Framework/RtWidgetActor_Radio.h"
#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtAIController.h"
#include "Framework/RtGuideHand.h"

#include "UI/Trainee/Rt_Trainee_Text.h"
#include "UI/Trainee/Rt_Trainee_SelectionMenu.h"
#include "UI/Trainee/Rt_Trainee_Selection.h"
#include "UI/Trainee/Rt_Trainee_Main.h"

#include "Network/RtNetworkManager.h"
#include "Manager/RtReplayManagerActor.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"

#include "Kismet/KismetMathLibrary.h"
#include "UObject/UnrealTypePrivate.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SphereComponent.h"

#include "glTFRuntimeFBXAssetActor.h"

DEFINE_LOG_CATEGORY(RtGameScenarioController);
#define SCRIPT_FUNCTION_CHECK if (/*nullptr == CurrentSctEventCommand || */nullptr == SceneManagerRef) {\
		retValue = false;\
		return;\
	}

ARtGameScenarioController::ARtGameScenarioController()
{
	SceneManagerRef = nullptr;

	StringToESctGameStateMap.Add(TEXT("GameState_None"), ERtSctGameState::EGameState_None);
	StringToESctGameStateMap.Add(TEXT("GameState_GotoReadyPlace"), ERtSctGameState::EGameState_GotoReadyPlace);
	StringToESctGameStateMap.Add(TEXT("GameState_AllArriveReadyPlace"), ERtSctGameState::EGameState_AllArriveReadyPlace);
	StringToESctGameStateMap.Add(TEXT("GameState_Play"), ERtSctGameState::EGameState_Play);
	StringToESctGameStateMap.Add(TEXT("GameState_Pause"), ERtSctGameState::EGameState_Pause);
	StringToESctGameStateMap.Add(TEXT("GameState_End"), ERtSctGameState::EGameState_End);

	Bind_DelegateCommands();

	SendLogMap.Add(ERtSctSendLogType::ELog_Challenge, ELogProtocol::LOG_CHALLENGE);
	SendLogMap.Add(ERtSctSendLogType::ELog_Surrender, ELogProtocol::LOG_SURRENDER);
	SendLogMap.Add(ERtSctSendLogType::ELog_Bind, ELogProtocol::LOG_BIND);
	SendLogMap.Add(ERtSctSendLogType::ELog_CommandMove, ELogProtocol::LOG_COMMANDMOVE);
	SendLogMap.Add(ERtSctSendLogType::ELog_BreifSituation, ELogProtocol::LOG_BREIFSITUATION);
	SendLogMap.Add(ERtSctSendLogType::ELog_Call, ELogProtocol::LOG_CALL);
	SendLogMap.Add(ERtSctSendLogType::ELog_CallEOD, ELogProtocol::LOG_EOD);
}

void ARtGameScenarioController::Bind_DelegateCommands()
{
	FDelegateCallCommand delegateCallCommand;

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsGameState);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsGameState, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsArriveGameReadyPlace);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsArriveGameReadyPlace, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::JustDoIt);
	CallCommandFunctionMap.Add(ERtSctFunCmd::JustDoIt, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsValue, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsOldValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsOldValue, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsVisible);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsVisible, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsAnimProgress);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsAnimProgress, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsTargetInteract);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsTargetInteract, delegateCallCommand);

	//delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsKeyUp);
	//CallCommandFunctionMap.Add(ERtSctFunCmd::IsKeyUp, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsInRadius);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsInRadius, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsTargetAimed);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsTargetAimed, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsHitObject);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsHitObject, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsIntimidatingFire);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsIntimidatingFire, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsTrainingElapsedAt);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsTrainingElapsedAt, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::CheckCollision);
	CallCommandFunctionMap.Add(ERtSctFunCmd::CheckCollision, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::ReqGameEnd);
	CallCommandFunctionMap.Add(ERtSctFunCmd::ReqGameEnd, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::NotDoing);
	CallCommandFunctionMap.Add(ERtSctFunCmd::NotDoing, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetGameState);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetGameState, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetEnableGameReadyPlace);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetEnableGameReadyPlace, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetValue, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::DelayTime);
	CallCommandFunctionMap.Add(ERtSctFunCmd::DelayTime, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetVisible);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetVisible, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetVisibleGroup);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetVisibleGroup, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::PlayAnim);
	CallCommandFunctionMap.Add(ERtSctFunCmd::PlayAnim, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::MoveToWayPoint);
	CallCommandFunctionMap.Add(ERtSctFunCmd::MoveToWayPoint, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::NpcMoveToWayPoint);
	CallCommandFunctionMap.Add(ERtSctFunCmd::NpcMoveToWayPoint, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::StopAct);
	CallCommandFunctionMap.Add(ERtSctFunCmd::StopAct, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::NpcInit);
	CallCommandFunctionMap.Add(ERtSctFunCmd::NpcInit, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::ShowScreenText);
	CallCommandFunctionMap.Add(ERtSctFunCmd::ShowScreenText, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::ShowTargetText);
	CallCommandFunctionMap.Add(ERtSctFunCmd::ShowTargetText, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::ShowScreenMenu);
	CallCommandFunctionMap.Add(ERtSctFunCmd::ShowScreenMenu, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::ShowTargetMenu);
	CallCommandFunctionMap.Add(ERtSctFunCmd::ShowTargetMenu, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::RemoveScreenText);
	CallCommandFunctionMap.Add(ERtSctFunCmd::RemoveScreenText, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::RemoveTargetText);
	CallCommandFunctionMap.Add(ERtSctFunCmd::RemoveTargetText, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::RemoveMenu);
	CallCommandFunctionMap.Add(ERtSctFunCmd::RemoveMenu, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::RecordResultLog);
	CallCommandFunctionMap.Add(ERtSctFunCmd::RecordResultLog, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::PlayMovie);
	CallCommandFunctionMap.Add(ERtSctFunCmd::PlayMovie, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::AttachWeapon);
	CallCommandFunctionMap.Add(ERtSctFunCmd::AttachWeapon, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::AttackTarget);
	CallCommandFunctionMap.Add(ERtSctFunCmd::AttackTarget, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetTargetMenuEnabled);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetTargetMenuEnabled, delegateCallCommand);

	// !! ���� !! : VoiceCommand �� ���� �Լ��� �ʿ� ����.

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SendLog);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SendLog, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsNotValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsNotValue, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsGreaterEqualValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsGreaterEqualValue, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::IsLessEqualValue);
	CallCommandFunctionMap.Add(ERtSctFunCmd::IsLessEqualValue, delegateCallCommand);

	// !! ���� !! : RequestRandomNum �� ���� �Լ��� ������� �ʴ´�.
	// ������ Force_RequestRandomNum �Լ��� �����Ѵ�.

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::SetVisibleRadioMenuItem);
	CallCommandFunctionMap.Add(ERtSctFunCmd::SetVisibleRadioMenuItem, delegateCallCommand);

	delegateCallCommand.BindUObject(this, &ARtGameScenarioController::EndTraining);
	CallCommandFunctionMap.Add(ERtSctFunCmd::EndTraining, delegateCallCommand);
}

void ARtGameScenarioController::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerID = TEXT("");
	bIsGamePlay = false;
	CurrentPlayType = ERt_PlayType::E_None;

	if (UGameInstance* gi = GetGameInstance())
	{
		if (URtConfig* config = gi->GetSubsystem<URtConfig>())
		{
			IsGuideAim = config->TrainingOptions.IsGuideAim;
			IntimidatingFire_Distance = config->TrainingOptions.IntimidatingFire_Distance;
			IntimidatingFire_Angle = config->TrainingOptions.IntimidatingFire_Angle;
		}
	}

	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (controller->Get_GameManager()->Is_Play_Type(ERt_PlayType::E_Single))
		{
			ARtPlayerState* playerState = controller->Get_PlayerState();
			playerState->Play_Type = ERt_PlayType::E_Single;

			IsSingleMode = true;
			controller->LoadSct();
		}
	}

	Regist_ConsoleCommands();
}

void ARtGameScenarioController::Regist_ConsoleCommands()
{
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Load"), TEXT("[SctFileName] [Role]"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.Num() >= 1)
			{
				if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					ARtGameState* state = controller->Get_GameState();
					if (controller->Get_PlayerState()->Play_Type != ERt_PlayType::E_Single)
						controller->Get_PlayerState()->Play_Type = ERt_PlayType::E_Trainee;
					FRtScenarioInfo sceInfo = state->Get_Scenario_Info();
					sceInfo.SCTFileName = FString::Printf(TEXT("%s.sct.json"), *Args[0]);
					state->Set_Scenario_Info(sceInfo);

					if (Args.Num() >= 2)
					{
						FRtDeviceInfo deviceInfo = controller->Get_PlayerState()->Get_PlayerInfo();
						deviceInfo.PlayerRoleID = *Args[1];
						controller->Get_PlayerState()->Set_PlayerInfo(deviceInfo);
					}

					controller->LoadSct();
				}
			}
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Play"), TEXT("[1/0] Play/Stop Sct"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					SetGamePlay(!bIsGamePlay);
				else
					SetGamePlay(Args[0].ToBool());
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Init"), TEXT("Initialize Scenario"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Scenario Init"));
				InitScenario();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.ConditionLog"), TEXT("[0/1] Toggle of Sct ConditionLog"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					return;

				ConditionLog = Args[0].ToBool();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct"), TEXT("Toggle Show of Sct"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				DebugMode = !DebugMode;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.All"), TEXT("[0/1] Toggle Show of All Sct Datas"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				bool debugMode = false;
				if (Args.IsEmpty())
					debugMode = true;
				else
					debugMode = Args[0].ToBool();

				DebugMode = debugMode;
				DebugValue = debugMode;
				DebugWayPoint = debugMode;
				DebugRadius = debugMode;
				DebugCollision = debugMode;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.Value"), TEXT("Toggle Show of Sct Values"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				DebugValue = !DebugValue;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.WayPoint"), TEXT("Toggle Show of Sct WayPoints"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				DebugWayPoint = !DebugWayPoint;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.Radius"), TEXT("Toggle Show of Sct IsInRadius Events"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				DebugRadius = !DebugRadius;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Debug.Sct.Collision"), TEXT("Toggle Show of Sct CheckCollision Events"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				DebugCollision = !DebugCollision;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Exec.Server"), TEXT("[ServerEventID] ExecuteServerEvent"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					return;

				if (auto* nmgr = Get_NetManager())
				{
					nmgr->SendPacket_SCTEventID(Args[0]);
				}
			}));
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Exec.Scenario"), TEXT("[ScenarioEventID] ExecuteScenarioEvent"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					return;

				auto* eventCommand = ScenarioModel->EventCommandMap.Find(*Args[0]);
				if (!eventCommand)
					return;

				// ��� ó�� �׸�� ����.
				for (const auto& command : eventCommand->ProcessCommandList)
				{
					if (command.FunctionCommand != ERtSctFunCmd::LoopEvent)
					{
						if (auto found = CallCommandFunctionMap.Find(command.FunctionCommand))
						{
							bool retValue = false;
							if (found->ExecuteIfBound(command.Params, retValue))
							{
							}
						}
					}
				}
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Debug.IntimidatingFire"), TEXT("[On/Off] On/Off"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					DebugIntimidatingFire = !DebugIntimidatingFire;
				else
					DebugIntimidatingFire = Args[0].ToBool();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Sct.Debug.Hit"), TEXT("[On/Off] On/Off"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.IsEmpty())
					DebugHit = !DebugHit;
				else
					DebugHit = Args[0].ToBool();
			}));
}

void ARtGameScenarioController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//TimerHandleMap.Empty();
	SetSctGamePlay.Clear();
}

void ARtGameScenarioController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld());
	if (gameSceneManager)
	{
		gameSceneManager->AsyncMapLoadProcess();
	}

	for (const FShotData& shotData : Resv_ShotList)
	{
		FString weaponID = shotData.WeaponID;
		if (!weaponID.IsEmpty())
		{
			if (ARtObjectActor* weapon = Cast<ARtObjectActor>(gameSceneManager->FindObjectByName(weaponID)))
			{
				weapon->PlaySound();
				weapon->PlaySingleEffect(ERtComponentType::EEffect, FVector::ZeroVector);
			}
		}
	}

	Process_ShotList = Resv_ShotList;
	Resv_ShotList.Empty();


	if (bIsGamePlay)
	{

		//ENetMode currentNetMode = GetWorld()->GetNetMode();
		CurrentSctEventCommand = nullptr;

		LineTraceAim();

		for (auto iter = Resv_AimMap.CreateIterator(); iter; ++iter)
		{
			Process_NewAimMap.Add(*iter);
			Process_AimingMap.Add(*iter);
			iter.RemoveCurrent();
			IsChangeAim = true;
		}

		// dave
		//if (nullptr != ScenarioModel)
		if (ScenarioModel.IsValid())
		{
			for (auto iter = Resv_InteractKeyList.CreateIterator(); iter; ++iter)
			{
				Process_InteractKeyList.Add(*iter);
				iter.RemoveCurrent();
			}

			/*
						for (auto iter = Resv_ShotList.CreateIterator(); iter; ++iter)
						{
							Process_ShotList.Add(*iter);
							iter.RemoveCurrent();
						}
			*/

			if (ScenarioModel->EventIDLoopList.Num() > 0)
			{
				TArray<FString> endEventReserveList;
				for (const auto& eventID : ScenarioModel->EventIDLoopList)
				{
					CurrentSctEventCommand = ScenarioModel->EventCommandMap.Find(eventID);
					if (CurrentSctEventCommand)
					{
						bool bIsLoop = ProcessOneEvent(CurrentSctEventCommand);
						if (!bIsLoop)
						{
							endEventReserveList.Add(eventID);
						}
					}
				}

				if (endEventReserveList.Num() > 0)
				{
					for (auto data : endEventReserveList)
					{
						ScenarioModel->EventIDLoopList.Remove(data);
					}

					endEventReserveList.Empty();
				}
			}
		//}

		//for (const FShotData& shotData : Process_ShotList)
		//{
		//	bool ammunitionMode = shotData.Ammunition;

		//	FString weaponID = shotData.WeaponID;
		//	/*
		//				if (!weaponID.IsEmpty())
		//				{
		//					if (ARtObjectActor* weapon = Cast<ARtObjectActor>(gameSceneManager->FindObjectByName(weaponID)))
		//					{
		//						weapon->PlaySound();
		//						weapon->PlaySingleEffect(ERtComponentType::EEffect, FVector::ZeroVector);
		//					}
		//				}
		//	*/

		//	if (GetNetMode() != NM_Client)
		//	{
		//		ARtObjectActor* weapon = gameSceneManager->GetRtObjectMap().FindRef(shotData.WeaponID);
		//		FVector startLoc = shotData.ShotPoint;
		//		FVector startDir = shotData.ShotDirection;
		//		FString objectID = shotData.HitObjectID;

		//		// ��ź ���� ��
		//		if (ammunitionMode)
		//		{
		//			if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(gameSceneManager->FindObjectByName(objectID)))
		//			{
		//				FRtNpcBehaviorState state;
		//				state.ActType = ERtAnimType::ECollapse;
		//				//npc->UpdateNpcBehaviorState(state);
		//			}
		//		}

		//		// ���� ��� üũ
		//		for (const TPair<FString, ARtNpcObjectActor*>& objectPair : gameSceneManager->GetRtNpcMap())
		//		{
		//			ARtNpcObjectActor* npc = objectPair.Value;
		//			if (!npc->IsVisible() ||
		//				npc->GetRoleType() == ERtRoleType::ERoleSoldier ||
		//				npc->GetNpcBehaviorState().ActType == ERtAnimType::ECollapse ||
		//				npc->GetNpcBehaviorState().ActType == ERtAnimType::ESurrender)
		//				continue;

		//			if (IsInRange(weapon, startLoc, startDir, npc, IntimidatingFire_Distance, IntimidatingFire_Angle))
		//			{
		//				FRtNpcBehaviorState state;
		//				state.ActType = ERtAnimType::ESurrender;
		//				//npc->UpdateNpcBehaviorState(state);
		//			}
		//		}
		//	}
		//}

		//		if (GetNetMode() == NM_ListenServer)
		//		{
		//			URtNetworkManager* nmgr = Get_NetManager();
		//
		//			for (TPair<FString, ARtNpcObjectActor*> npcPair : gameSceneManager->GetRtNpcMap())
		//			{
		//				ARtNpcObjectActor* npc = npcPair.Value;
		//				if (npc->GetIsUpdateAct())
		//				{
		//					npc->SetIsUpdateAct(false);
		//
		//					FString npcName = npc->GetObjectName();
		//					if (npcName.Len() <= 64)
		//					{
		//
		//						const FRtNpcBehaviorState& bhState = npc->GetNpcBehaviorState();
		//						stNpcState packetState;
		//
		//						const wchar_t* charArray_NpcID = TCHAR_TO_WCHAR(*npcName);
		//						FMemory::Memcpy(packetState.NpcID, charArray_NpcID, npcName.Len() * 2);
		//
		//						if (bhState.SctFunCmd == ERtSctFunCmd::MoveToWayPoint ||
		//							bhState.SctFunCmd == ERtSctFunCmd::NpcMoveToWayPoint)
		//						{
		//							packetState.State = (int8)bhState.MoveType;
		//						}
		//						else
		//						{
		//							packetState.State = (int8)bhState.ActType;
		//						}
		//
		//						packetState.SctFunCmd = (int8)bhState.SctFunCmd;
		//
		//						if (bhState.ActType != ERtAnimType::ECollapse &&
		//							bhState.ActType != ERtAnimType::ESurrender &&
		//							bhState.ActType != ERtAnimType::EAttack)
		//						{
		//
		//							switch (bhState.SctFunCmd)
		//							{
		//							case ERtSctFunCmd::PlayAnim:
		//							{
		//								FString data2 = bhState.SctString;
		//								const wchar_t* charArray_Data2 = TCHAR_TO_WCHAR(*data2);
		//								FMemory::Memcpy(packetState.Data2.SctString, charArray_Data2, data2.Len() * 2);
		//
		//								packetState.Data3.SctBool = bhState.SctBool;
		//							}
		//							break;
		//							default:
		//							{
		//								UObject* targetKey = npc->GetAIController()->GetBlackboardTarget();
		//								if (npc->GetAIController()->GetMoveStatus() == EPathFollowingStatus::Moving)
		//								{
		//									packetState.Speed = npc->GetSpeed();
		//									if (targetKey == nullptr)
		//									{
		//										FVector freeMove = npc->GetAIController()->GetBlackboardFreeMove();
		//										packetState.Data1.SctFloat = freeMove.X;
		//										packetState.Data2.SctFloat = freeMove.Y;
		//										packetState.Data3.SctFloat = freeMove.Z;
		//									}
		//								}
		//
		//								if (targetKey)
		//								{
		//									packetState.Speed = 0.0f;
		//									if (ARtObjectActor* targetObj = Cast<ARtObjectActor>(targetKey))
		//									{
		//										FString data1 = targetObj->GetObjectName();
		//										const wchar_t* charArray_Data1 = TCHAR_TO_WCHAR(*data1);
		//										FMemory::Memcpy(packetState.Data1.SctString, charArray_Data1, data1.Len() * 2);
		//									}
		//									else if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(targetKey))
		//									{
		//										ARtPlayerState* playerState = targetTrainee->Get_PlayerState();
		//										FString data1 = playerState->Get_PlayerInfo().PlayerRoleID;
		//										const wchar_t* charArray_Data1 = TCHAR_TO_WCHAR(*data1);
		//										FMemory::Memcpy(packetState.Data1.SctString, charArray_Data1, data1.Len() * 2);
		//
		//									}
		//
		//								}
		//							}
		//							break;
		//							}
		//						}
		//
		//						if (DebugMode)
		//						{
		//							FString msg = FString::Printf(TEXT("Send: %s / %d, %d, %f"), *npcName, packetState.SctFunCmd, packetState.State, packetState.Speed);
		//							GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, msg);
		//						}
		//
		//						//		packetState.State = 2;
		//#if WITH_EDITOR
		//						FString targetName = (TCHAR*)packetState.Data1.SctString;
		//						FILE_APP_LOG("server_debug.log",
		//							"ARtGameScenarioController::Tick() SendPacket : %s / %s(%d), %s(%d), %f, tar : %s",
		//							*npcName,
		//							*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtSctFunCmd"), packetState.SctFunCmd), packetState.SctFunCmd,
		//							*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), packetState.State), packetState.State,
		//							packetState.Speed, *targetName
		//						);
		//#endif						
		//						nmgr->SendPacket_NpcState(packetState);
		//					}
		//				}
		//			}
		//		}

			IsChangeAim = false;
			Process_NewAimMap.Empty();
			Process_InteractKeyList.Empty();
			//Process_ShotList.Empty();

			TArray<FString> removeChangedValues;
			for (TPair<FString, TPair<int, int>> data : NewVarialbleMap)
			{
				if (data.Value.Value != ScenarioModel->VarialbleMap.FindRef(data.Key))
				{ // �� ������ ����
					ScenarioModel->OldVarialbleMap.Add(data.Key, data.Value.Key);
					ScenarioModel->VarialbleMap.Add(data.Key, data.Value.Value);
				}
				else
				{ // ����� ���� ƽ�̹Ƿ� ���� ���� �� ������ ����
					ScenarioModel->OldVarialbleMap.Add(data.Key, data.Value.Value);
					removeChangedValues.Add(data.Key);
				}
			}
			for (FString removeKey : removeChangedValues)
			{
				NewVarialbleMap.Remove(removeKey);
			}
			MoveToWayPoint_Progress();

			if (CurrentPlayType == ERt_PlayType::E_Server) {
				AllNpcPosTimer -= DeltaTime;
				if (AllNpcPosTimer < 0) {
					if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
					{
						if (ARtGameState* gameState = controller->Get_GameState())
						{
							gameState->Set_OnUpdate_AllNpcPos();
						}
					}
					AllNpcPosTimer = ALL_NPC_UPDATE_POS_TIMER_TIME;
				}
			}
		}

		TrainingTimer += DeltaTime;
	}

	if (DebugMode)
	{
		const uint64 ScreenKey = GetTypeHash(FName("SctDebugMsg"));
		FString LogString = FString::Printf(TEXT("========================================\n SctDebugMsg\n"));

		LogString.Append(FString::Printf(TEXT("SctFileName : %s\n"), *SctFileName));
		LogString.Append(FString::Printf(TEXT("PlayType : %s\n"), *UEnum::GetValueAsString(CurrentPlayType)));
		LogString.Append(FString::Printf(TEXT("Role : %s\n"), *CurrentPlayerID));
		LogString.Append(FString::Printf(TEXT("Aim : %s\n"), *RifleAimedObjectKeyName));

		LogString.Append(FString::Printf(TEXT("Events : {\n")));

		// dave
		if (ScenarioModel.IsValid()) {
			int i = 0;
			for (TPair<FString, int> pair : DebugCondition)
			{
				FRtScenarioEventCommand* command = ScenarioModel->EventCommandMap.Find(pair.Key);

				LogString.Append(FString::Printf(TEXT("%s(%d/%d), "), *pair.Key, pair.Value, command ? command->ConditionCommandList.Num() : 0));

				++i;
				if (i > 3)
				{
					LogString.Append(FString::Printf(TEXT("\n")));
					i = 0;
				}
			}
		}
		else 
		{
			LogString.Append(FString::Printf(TEXT("*** ScenarioModel is NOT Valid.\n")));
		}
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		DebugCondition.Empty();
#endif

		if (DebugValue)
		{
			LogString.Append(TEXT("\n [Value]\n"));
			if (ScenarioModel.IsValid())
			{
				for (auto data : ScenarioModel->VarialbleMap)
				{
					LogString.Append(FString::Printf(TEXT("%s : %d\n"), *data.Key, data.Value));
				}
			}
			else
			{
				LogString.Append(FString::Printf(TEXT("*** ScenarioModel is NOT Valid.\n")));
			}
		}
		if (DebugWayPoint)
		{
			LogString.Append(TEXT("\n [MoveToWayPoint]\n"));
			for (auto data : MoveToWayPointMap)
			{
				ARtObjectActor* moveActor = Cast<ARtObjectActor>(data.Key);
				AActor* wp = data.Value.Key;
				LogString.Append(FString::Printf(TEXT("%s : %s : %f\n"), *(gameSceneManager->GetComponentObjectNameKey(moveActor)), *(gameSceneManager->GetComponentObjectNameKey(wp)), FVector::Distance(moveActor->GetActorLocation(), wp->GetActorLocation())));
			}
		}
		LogString.Append(TEXT("\n========================================\n"));
		GEngine->AddOnScreenDebugMessage(ScreenKey, 5.f, FColor::Magenta, LogString);
	}
	else
		GEngine->RemoveOnScreenDebugMessage(GetTypeHash(FName("SctDebugMsg")));
}

// ������ true�̸� �̺�Ʈ ����, ������ false�̸� �̺�Ʈ ����
bool ARtGameScenarioController::ProcessOneEvent(FRtScenarioEventCommand* scenarioEventCommand)
{
	if (scenarioEventCommand)
	{
		//bool bDoRun = false;
		//FRtScenarioEvent* pScenarioEvent = GetScenarioEvent(scenarioEventCommand->EventDataIndex);
		//if (pScenarioEvent)
		//{
		//	bDoRun = true;
		//
		//	//if (pScenarioEvent->EventHandleType == ERtEventHandleType::EClient ||
		//	//	pScenarioEvent->EventHandleType == ERtEventHandleType::EListenServer || // ���������� Ŭ���̾�Ʈ�� ���Ե�.
		//	//	pScenarioEvent->EventHandleType == ERtEventHandleType::EServerClient)
		//	//{
		//	//	if (CurrentPlayType == ERt_PlayType::E_Trainee || CurrentPlayType == ERt_PlayType::E_Control || CurrentPlayType == ERt_PlayType::E_Observer)
		//	//	{
		//	//		
		//	//	}
		//	//}
		//	//else if (pScenarioEvent->EventHandleType == ERtEventHandleType::EListenServer)
		//	//{
		//	//	if (CurrentPlayType == ERt_PlayType::E_Server)
		//	//	{
		//	//		bDoRun = true;
		//	//	}
		//	//}
		//}
		//
		//if (!bDoRun)
		//{
		//	return true;
		//}

		// �̺�Ʈ ������ �ִ°�?
		if (scenarioEventCommand->ConditionCommandList.Num() > 0)
		{
			int conditionOkCounter = 0;
			// �̺�Ʈ ���ǿ� �´��� üũ.
			for (const auto& command : scenarioEventCommand->ConditionCommandList)
			{
				auto found = CallCommandFunctionMap.Find(command.FunctionCommand);
				if (found)
				{
					bool retValue = false;
					if (found->ExecuteIfBound(command.Params, retValue))
					{
						if (retValue == true)
						{
							conditionOkCounter++;
						}
					}
				}
			}

			DebugCondition.Add(scenarioEventCommand->EventID, conditionOkCounter);

			// �̺�Ʈ ������ ��� �´ٸ�..
			if (conditionOkCounter == scenarioEventCommand->ConditionCommandList.Num())
			{
				bool bIsLoop = false;

				ARtPlayerState* myPlayerState = GetMyPlayerState();
				if (myPlayerState) {
					FString playerInfo = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)myPlayerState->Play_Type);
					if (myPlayerState->Play_Type == ERt_PlayType::E_Trainee)
					{
						playerInfo = myPlayerState->Login_DeviceName;
					}

					FString msg = FString::Printf(TEXT("[%s][SCT Conditions completed Event : %s]"), 
						*playerInfo, *scenarioEventCommand->EventID);
					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
					FILE_APP_LOG("App.log", "%s", *msg);

					// �̺�Ʈ ���� �α�
					for (FRtScenarioEvent eventData : ScenarioModel->SctData.EventList)
					{
						if (eventData.EventNameTID == scenarioEventCommand->EventID)
						{
							for (FRtScenarioCondition condition : eventData.EventOccurConditionList)
							{
								msg = FString::Printf(TEXT("-> Condition Script : %s"), *condition.ConditionScript);
								UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
								FILE_APP_LOG("App.log", "%s", *msg);
							}
						}
					}
				}

				Force_RequestRandomNum(&scenarioEventCommand->ProcessCommandList);

				bool isReceive = false;
				if (!scenarioEventCommand->bLocalProcessOnControl) { 
					Get_NetManager()->SendPacket_SCTEventID(scenarioEventCommand->EventID);
				} else { // Ư���� ���������� ó���� �ʿ��� ���..
					if (CurrentPlayType == ERt_PlayType::E_Control) {
						isReceive = true;
					} else {
						return false;
					}
				}

				return ExecuteProcessCommands(scenarioEventCommand, isReceive);
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

void ARtGameScenarioController::SetRifleTargetObjectKeyName(bool bIsAimed, FString objectKeyName, FVector hitLocation)
{
	APP_LOG("SetRifleAimedObjectKeyName %s", *objectKeyName);

	if (bIsAimed)
	{
		RifleAimedObjectKeyName = objectKeyName;
	}
	else
	{
		RifleShotObjectKeyName = objectKeyName;
	}
	RifleAimedHitLocation = hitLocation;
}

void ARtGameScenarioController::Init(ERt_PlayType currentPlayType, FString sctFileName)
{
	CurrentPlayType = currentPlayType;
	SctFileName = sctFileName;

	// �ӽ÷� �۾��� �ڵ���. sct ���ϸ����� ������ ���� �ؾ���.
	//SctFileName = SctFileName.Replace(TEXT("mct"), TEXT("sct"));
}

void ARtGameScenarioController::IsGameState(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringGameState)
			{
				auto found = StringToESctGameStateMap.Find(param[0].ParamString);
				if (found)
				{
					if (ScenarioModel->GameState == (*found))
					{
						FString msg = FString::Printf(TEXT("SCT IsGameState: %s"), *param[0].ParamString);
						//const uint64 ScreenKey = GetTypeHash(FName(msg));
						//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Blue, msg);
						if (ConditionLog)
							UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);

						retValue = true;
						return;
					}
				}
			}
		}

	retValue = false;
}

// ���� ���� �غ� ��ҿ� �����ߴ��� ���θ� Ȯ��
void ARtGameScenarioController::IsArriveGameReadyPlace(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID)
			{
				TArray<FString> objectIDList = GetObjectIDList(param[0].ParamString);
				if (objectIDList.Num() > 0)
				{
					for (auto& objectID : objectIDList)
					{
						if (objectID == CurrentPlayerID)
						{
							ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(GetSceneManagerRef()->FindObjectByName(MyScenarioPlayer.GameReadyPlaceObjectID));
							if (pObjectActor)
							{
								if (pObjectActor->FindOverlapedObjectName(CurrentPlayerID))
								{
									retValue = true;
									return;
								}
							}
						}
					}
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::JustDoIt(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = true;
}

void ARtGameScenarioController::IsValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->VarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					retValue = (*value == param[1].ParamInt);
					if (ConditionLog) {
						FString msg = FString::Printf(TEXT("SCT IsValue: %s, %d"), *param[0].ParamString, param[1].ParamInt);
						FString log = msg + FString(retValue ? TEXT("True") : TEXT("False"));
						//const uint64 ScreenKey = GetTypeHash(FName(msg));
						//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, log);
						UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *log);
					}
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::IsOldValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK
		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->OldVarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					retValue = (*value == param[1].ParamInt);
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::IsVisible(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ��� ID
				param[1].ParamType == ERtSctFunCmdParam::EIntValue) // ���̴���(1) �Ⱥ��̴���(0)
			{
				FString objectID = param[0].ParamString;
				bool checkVisible = param[1].ParamInt != 0;

				TArray<FString> whoIDList = GetObjectIDList(objectID);
				if (whoIDList.IsEmpty())
					return;
				ARtObjectActor* actor = GetSceneManagerRef()->FindObjectByName(whoIDList[0]);
				if (actor == nullptr)
					return;
				if (actor->IsVisible() == checkVisible)
				{
					retValue = true;
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::IsAnimProgress(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() >= 4)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && //��� ���̷��� ��ü ID
				(param[1].ParamType == ERtSctFunCmdParam::EStringAniName || //�ִϸ��̼� �̸�
					param[2].ParamType == ERtSctFunCmdParam::EIntValue) && //AnimType)
				param[3].ParamType == ERtSctFunCmdParam::EFloatValue) //�ִϸ��̼� ���൵
			{
				TArray<FString> whoIDList = GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
					return;
				AActor* actor = GetSceneManagerRef()->FindObjectByName(whoIDList[0]);
				ARtObjectActor* rtObjectActor = Cast<ARtObjectActor>(actor);
				if (rtObjectActor == nullptr)
					return;

				FString animName = param[1].ParamString;
				//ERtAnimType animType = (ERtAnimType)param[2].ParamInt;

				FString findAnimPath = "";

				if (animName.IsEmpty())
				{
					ARtNpcObjectActor* pNPC = Cast<ARtNpcObjectActor>(rtObjectActor);

					if (pNPC)
						findAnimPath = pNPC->Find_AnimName((ERtAnimType)param[2].ParamInt);
				}
				else
					findAnimPath = RtSctPath::RtAnimationPath + FString::Printf(TEXT("/%s.%s"), *animName, *animName);

				TPair<FString, float> animProgress = rtObjectActor->GetAnimationProgress();
				bool result = (animProgress.Key == findAnimPath) && animProgress.Value >= param[3].Paramfloat;

				FString msg = FString::Printf(TEXT("SCT IsAnimProgress: %s, %s, %f : "), *param[0].ParamString, *animName, param[3].Paramfloat);
				FString log = msg + (result ? TEXT("True") : TEXT("False"));
				//const uint64 ScreenKey = GetTypeHash(FName(msg));
				//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, log);
				if (ConditionLog)
					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *log);

				retValue = result;
				return;
			}
		}
	retValue = false;
}

//IsKeyUp,A
// ���� : Ű�� ������ �� ��ȣ üũ
// �Ķ���� : Ư�� Ű ��.
void ARtGameScenarioController::IsTargetInteract(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK
		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID) // TargetID
			{
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
				// [ �Է°� ���۷��� ]
				// param[0].ParamString �� InputCoreTypes.cpp �� �ִ� ��Ʈ������ �Է����� ���� �Ѵ�.
				// const FKey EKeys::SpaceBar("SpaceBar");
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				FString targetsID = param[0].ParamString;

				TArray<FString> whoIDList = GetObjectIDList(targetsID);
				if (whoIDList.IsEmpty())
					return;

				if (Process_InteractKeyList.IsEmpty())
					return;

				for (int32 playerID : Process_InteractKeyList)
				{
					if (FString* ptrAimTarget = Process_AimingMap.Find(playerID))
					{
						for (FString targetID : whoIDList)
						{
							if (targetID == *ptrAimTarget)
							{
								FString msg = FString::Printf(TEXT("SCT IsTargetInteract: %s"), *targetID);
								//const uint64 ScreenKey = GetTypeHash(FName(msg));
								//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);
								if (ConditionLog)
									UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
								retValue = true;
								return;
							}
						}
					}
				}

				//for (FString targetID : whoIDList)
				//{
				//	if (RifleAimedObjectKeyName != targetID)
				//		return;
				//
				//	FKey keyValue = FKey(*InputKey);
				//	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(keyValue))
				//	{
				//		FString msg = FString::Printf(TEXT("SCT IsTargetInteract %s %s"), *targetID, *InputKey);
				//		//const uint64 ScreenKey = GetTypeHash(FName(msg));
				//		//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);
				//		UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
				//		retValue = true;
				//		return;
				//	}
				//}
			}
		}

	retValue = false;
}

//IsKeyUp,A
// ���� : Ű�� �����ٰ� ���� ��ȣ üũ
// �Ķ���� : Ư�� Ű ��.
//void ARtGameScenarioController::IsKeyUp(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
//{
//	SCRIPT_FUNCTION_CHECK
//
//		if (param.Num() == 1)
//		{
//			if (param[0].ParamType == ERtSctFunCmdParam::EStringKeyValue)
//			{
//				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//				// [ �Է°� ���۷��� ]
//				// param[0].ParamString �� InputCoreTypes.cpp �� �ִ� ��Ʈ������ �Է����� ���� �Ѵ�.
//				// const FKey EKeys::SpaceBar("SpaceBar");
//				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				FKey keyValue = FKey(*param[0].ParamString);
//				if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(keyValue))
//				{
//					FString msg = FString::Printf(TEXT("SCT IsKeyUp %s"), *param[0].ParamString);
//					//const uint64 ScreenKey = GetTypeHash(FName(msg));
//					//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);
//					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
//					retValue = true;
//					return;
//				}
//			}
//		}
//
//	retValue = false;
//}

void ARtGameScenarioController::IsInRadius(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() >= 3)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ��üA ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID && // ��üB ID
				param[2].ParamType == ERtSctFunCmdParam::EFloatValue) // �Ÿ�
			{
				FString actorA_ID = param[0].ParamString;
				FString actorB_ID = param[1].ParamString;
				float radius = param[2].Paramfloat;

				FString msg = FString::Printf(TEXT("SCT IsInRadius: %s, %s, %f"), *actorA_ID, *actorB_ID, radius);

				TArray<FString> objectA_IDList = GetObjectIDList(actorA_ID);
				if (objectA_IDList.IsEmpty())
					return;
				TArray<FString> objectB_IDList = GetObjectIDList(actorB_ID);
				if (objectB_IDList.IsEmpty())
					return;

				TFunction<void(AActor*&, FVector&, const FString)> findObjLoc = [&](AActor*& actor, FVector& loc, FString id) {
					if (ARtObjectActor* rtObjectActorA = GetSceneManagerRef()->FindObjectByName(id))
					{
						actor = rtObjectActorA;
						loc = actor->GetActorLocation();
					}
					else
					{
						if (ARtTraineePawn* traineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(id))
						{
							// dave
							if (IsValid(traineePawn) && traineePawn->IsValidLowLevel())
							{
								actor = traineePawn;
								loc = traineePawn->Camera->GetComponentLocation();
							}
							else 
							{
								actor = nullptr;
							}
						}

					}
					};

				for (FString objectA_ID : objectA_IDList)
				{
					AActor* actorA = nullptr;
					FVector LocationA = FVector::ZeroVector;

					findObjLoc(actorA, LocationA, objectA_ID);

					if (actorA == nullptr)
					{
						//UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *objectA_ID);
						continue;
					}

					for (FString objectB_ID : objectB_IDList)
					{
						AActor* actorB = nullptr;
						FVector LocationB = FVector::ZeroVector;

						findObjLoc(actorB, LocationB, objectB_ID);
						if (actorB == nullptr)
						{
							//UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *objectB_ID);
							continue;
						}

						LocationA.Z = 0.0f;
						LocationB.Z = 0.0f;
						float distance = FVector::Distance(LocationA, LocationB);

						if (DebugRadius)
						{
							const uint64 screenKey = GetTypeHash(FName(FString::Printf(TEXT("%s , %s"), *objectA_ID, *objectB_ID)));
							FString debugMsg = FString::Printf(TEXT("%s , %s : %f / %f"),
								*objectA_ID, *objectB_ID, distance, radius);
							GEngine->AddOnScreenDebugMessage(screenKey, 10.f, FColor::Orange, debugMsg);
						}

						if (distance <= radius)
						{
							//const uint64 ScreenKey = GetTypeHash(FName(msg));
							//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);
							if (ConditionLog)
								UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
							retValue = true;
							return;
						}
					}
				}

			}
		}
	retValue = false;
}

// IsTargetAimed,{TargetID},{OwnerID}
// ���� : ���� ����(Ȥ�� ��ü)�� �����ϴ� ��� Ȯ���ϴ� �Լ�.	
// �Ķ���� : ����,	����(Ȥ�� ��ü)��
void ARtGameScenarioController::IsTargetAimed(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2 && IsChangeAim) // Aim�� �ٲ� ��쿡�� üũ
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ���� �޴� ��� ID
				param[1].ParamType == ERtSctFunCmdParam::EIntValue) // ������ ������(1) or �ƹ��� �������� ����(0)
			{
				FString targetID = param[0].ParamString;
				bool checkAim = param[1].ParamInt > 0; // ������ ������(1) or �ƹ��� �������� ����(0)

				FString msg = FString::Printf(TEXT("SCT IsTargetAimed: %s, %s"), *targetID, checkAim ? TEXT("True") : TEXT("False"));
				//const uint64 ScreenKey = GetTypeHash(FName(msg));
				//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);

				TArray<FString> whoIDList = GetObjectIDList(targetID);
				if (checkAim)
				{
					for (auto object : whoIDList)
					{
						for (auto aimData : Process_NewAimMap)
						{
							retValue = (object == aimData.Value);
							if (retValue)
							{
								if (ConditionLog)
									UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
								return;
							}
						}
					}
				}
				else
				{
					bool isAimed = false;
					for (auto object : whoIDList)
					{
						for (auto aimData : Process_AimingMap)
						{
							isAimed = (object == aimData.Value); // ������ ����� ������ isAimed = true
							if (isAimed)
								return;
						}
					}
					retValue = true;
					if (ConditionLog)
						UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
					return;
				}
			}
		}


	retValue = false;
}

void ARtGameScenarioController::IsHitObject(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

	retValue = false;
	if (param.Num() == 2)
	{
		if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ��� ��� ObjectID
			param[1].ParamType == ERtSctFunCmdParam::EIntValue) // ���� ����
		{

			FString targetID = param[0].ParamString;
			bool isHit = param[1].ParamInt > 0;

			FString msg = FString::Printf(TEXT("SCT IsHitObject: %s, %s"), *targetID, isHit ? TEXT("True") : TEXT("False"));

			TArray<FString> whoIDList = GetObjectIDList(targetID);
			if (whoIDList.IsEmpty())
				return;

			AActor* target = GetSceneManagerRef()->FindObjectByName(whoIDList[0]);
			if (target == nullptr)
			{
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *targetID);
				return;
			}

			if (Process_ShotList.IsEmpty())
				return;

			bool checkHit = false;

			for (const FShotData& shotData : Process_ShotList)
			{
				if (targetID == shotData.HitObjectID && shotData.Ammunition) // Ÿ���� ���� ���
				{
					checkHit = true;
				}

				if (DebugHit)
					DrawDebugSphere(GetWorld(), shotData.HitPoint, 1, 32, FColor::Red, false, 1, 0U, 2);
			}

			retValue = (checkHit == isHit);
			return;
		}
	}
}

void ARtGameScenarioController::IsIntimidatingFire(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 3)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EIntValue && // ��ź(1)/����ź(0) ����
				param[1].ParamType == ERtSctFunCmdParam::EStringID && // ������� ��� ObjectID
				param[2].ParamType == ERtSctFunCmdParam::EFloatValue) // ����������� �Ǵ��� ��� ������ ����� �Ÿ�
			{
				retValue = false;

				int32 paramAmmunitionMode = param[0].ParamInt; // ��ź(1)/����ź(0)
				FString targetID = param[1].ParamString;
				float allowDistance = param[2].Paramfloat;

				FString msg = FString::Printf(TEXT("SCT IsIntimidatingFire: %d, %s, %f"), paramAmmunitionMode, *targetID, allowDistance);

				TArray<FString> whoIDList = GetObjectIDList(targetID);
				if (whoIDList.IsEmpty())
					return;

				AActor* target = GetSceneManagerRef()->FindObjectByName(whoIDList[0]);
				if (target == nullptr)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *targetID);
					return;
				}

				float minDist = TNumericLimits<float>::Max();
				
				FVector boundOrigin, boundExtent, closetPoint;
				target->GetActorBounds(true, boundOrigin, boundExtent);
				FShotData resultShot("", "", FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector, false);

				bool ammunitionMode = false;
				for (const FShotData& shotData : Process_ShotList)
				{
					ammunitionMode = shotData.Ammunition;
					FString objectID = shotData.HitObjectID;

					if (targetID == objectID) // Ÿ���� ���� ���
						return;

					FVector closest;
					float distance = FMath::PointDistToLine(boundOrigin, shotData.ShotDirection, shotData.ShotPoint, closest);
					if (DebugIntimidatingFire && distance < minDist)
					{
						resultShot = shotData;
						closetPoint = closest;
					}

					minDist = FMath::Min(distance, minDist);
				}

				if (minDist <= allowDistance)
				{
					if (DebugIntimidatingFire)
					{
						FVector lineEnd = resultShot.HitObjectID.IsEmpty() ? resultShot.ShotPoint + resultShot.ShotDirection * 10000 : resultShot.HitPoint;
						DrawDebugLine(GetWorld(), resultShot.ShotPoint, lineEnd, FColor::Green, false, 2, 0U, 5);
						DrawDebugLine(GetWorld(), boundOrigin, closetPoint, FColor::Blue, false, 2, 0U, 5);
						DrawDebugBox(GetWorld(), boundOrigin, boundExtent, target->GetActorRotation().Quaternion(), FColor::Red, false, 2, 0U, 5);

						FString printstr = FString::Printf(TEXT("IntimidatingFire Distance %fcm"), minDist);
						UKismetSystemLibrary::PrintString(this, *printstr, true, true, FLinearColor::Red, 2, "IntimidatingFire");
					}

					if (paramAmmunitionMode) 
					{ // ������ ��ź �ΰ�?
						retValue = ammunitionMode;
					} else { // ������ ����ź�ΰ�?
						retValue = !ammunitionMode;
					}

//					retValue = true;
					return;
				}
			}
		}

	retValue = false;
}

// �Ʒ� ���� �� �� �ʰ� �귶���� ����Ͽ� �� �ð��� �帣�� ���� ��.
void ARtGameScenarioController::IsTrainingElapsedAt(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if ( param[0].ParamType == ERtSctFunCmdParam::EIntValue ) // ��� �ð�(��)
			{
				if ((float)param[0].ParamInt <= TrainingTimer) // �ش� �ð��� ��� �� ���..
				{
					retValue = true;
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::CheckCollision(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() >= 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // �浹 ��ü ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID) // �ݸ����ڽ� ID
			{
				FString objectGroup_ID = param[0].ParamString;
				FString collisionBoxGroup_ID = param[1].ParamString;

				FString msg = FString::Printf(TEXT("SCT CheckCollision: %s, %s"), *objectGroup_ID, *collisionBoxGroup_ID);

				TFunction<void(AActor*&, const FString, TArray<UPrimitiveComponent*>&)> findObj = [&](AActor*& actor, FString id, TArray<UPrimitiveComponent*>& collisions) {
					if (ARtObjectActor* rtObjectActorA = GetSceneManagerRef()->FindObjectByName(id))
					{
						actor = rtObjectActorA;
						for (auto collision : rtObjectActorA->GetCollisions())
						{
							collisions.Add(collision);
						}
					}
					else
					{
						if (ARtTraineePawn* traineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(id))
						{
							// dave
							if (IsValid(traineePawn) && traineePawn->IsValidLowLevel())
							{
								actor = traineePawn;
								collisions.Add(traineePawn->SkeletalMesh);
							}
							else 
							{
								actor = nullptr;
							}
						}
					}
					};

				TArray<FString> object_IDList = GetObjectIDList(objectGroup_ID);
				if (object_IDList.IsEmpty())
					return;
				TArray<FString> collisionBox_IDList = GetObjectIDList(collisionBoxGroup_ID);
				if (collisionBox_IDList.IsEmpty())
					return;

				for (FString object_ID : object_IDList)
				{
					AActor* actor = nullptr;
					TArray<UPrimitiveComponent*> collisionsA;

					findObj(actor, object_ID, collisionsA);

					if (actor == nullptr)
					{
						UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *object_ID);
						continue;
					}

					for (FString collisionBox_ID : collisionBox_IDList)
					{
						AActor* collisionBoxActor = nullptr;
						TArray<UPrimitiveComponent*> collisionsB;
						findObj(collisionBoxActor, collisionBox_ID, collisionsB);

						if (collisionBoxActor == nullptr)
						{
							UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s is Not Found"), *msg, *collisionBox_ID);
							continue;
						}

						bool isCollision = false;
						for (UPrimitiveComponent* collisionA : collisionsA)
						{
							TSet<AActor*> overlappingActors;
							collisionA->GetOverlappingActors(overlappingActors);
							if (overlappingActors.Contains(collisionBoxActor))
							{
								isCollision = true;
								break;
							}
						}
						if (!isCollision)
						{
							for (UPrimitiveComponent* collisionB : collisionsB)
							{
								TSet<AActor*> overlappingActors;
								collisionB->GetOverlappingActors(overlappingActors);
								if (overlappingActors.Contains(actor))
								{
									isCollision = true;
									break;
								}
							}
						}

						if (DebugCollision)
						{
							const uint64 screenKey = GetTypeHash(FName(FString::Printf(TEXT("%s , %s"), *object_ID, *collisionBox_ID)));
							FString debugMsg = FString::Printf(TEXT("%s , %s : %s"),
								*object_ID, *collisionBox_ID, isCollision ? TEXT("True") : TEXT("False"));
							GEngine->AddOnScreenDebugMessage(screenKey, 10.f, isCollision ? FColor::Orange : FColor::Cyan, debugMsg);
						}

						if (isCollision)
						{
							//const uint64 ScreenKey = GetTypeHash(FName(msg));
							//GEngine->AddOnScreenDebugMessage(ScreenKey, 10.f, FColor::Orange, msg);
							if (ConditionLog)
								UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
							retValue = true;
							return;
						}
					}
				}

			}
		}

	retValue = false;
}

void ARtGameScenarioController::ReqGameEnd(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = false;
}

void ARtGameScenarioController::NotDoing(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = true;
}

void ARtGameScenarioController::SetGameState(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	if (param.Num() == 1)
	{
		if (param[0].ParamType == ERtSctFunCmdParam::EStringGameState)
		{
			auto found = StringToESctGameStateMap.Find(param[0].ParamString);
			if (found)
			{
				ScenarioModel->GameState = (*found);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *param[0].ParamString);
				CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *param[0].ParamString);
				retValue = true;
				return;
			}
		}
	}

	retValue = false;
}

// ���� �غ� ��� ��ũ �����ֱ�
void ARtGameScenarioController::SetEnableGameReadyPlace(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				TArray<FString> objectIDList = GetObjectIDList(param[0].ParamString);
				if (objectIDList.Num() > 0)
				{
					for (auto& objectID : objectIDList)
					{
						if (objectID == CurrentPlayerID)
						{
							if (SceneManagerRef.IsValid())
							{
								ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(GetSceneManagerRef()->FindObjectByName(MyScenarioPlayer.GameReadyPlaceObjectID));
								if (pObjectActor)
								{
									//pObjectActor->SetVisible(param[1].ParamInt==1);								
									pObjectActor->SetDecal_StartPointNotReady(!(param[1].ParamInt == 1));

									retValue = false;
									return;
								}
							}
						}
					}
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::SetValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->VarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					int32 prevValue = *value;
					int32 newValue = param[1].ParamInt;
					retValue = newValue != prevValue;
					if (retValue)
					{
						NewVarialbleMap.Add(*param[0].ParamString, { prevValue, newValue });
					}

					FString msg = FString::Printf(TEXT("SCT SetValue: %s, %d"), *param[0].ParamString, param[1].ParamInt);
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);

					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::DelayTime(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = false;
}

void ARtGameScenarioController::SetVisible(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID &&//��� ID
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)//�����ֱ�(bool)
			{
				TArray<FString> whoIDList = GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					ARtObjectActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);
					if (actor == nullptr)
						continue;

					bool prevVisible = actor->IsVisible();
					bool newVisible = param[1].ParamInt != 0;
					actor->SetVisible(newVisible);

					FString msg = FString::Printf(TEXT("SCT SetVisible: %s, %d : %s"), *param[0].ParamString, param[1].ParamInt, *objectID);
					//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
					retValue = prevVisible != newVisible;
				}
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::SetVisibleGroup(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = false;
}

void ARtGameScenarioController::PlayAnim(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 4)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && //��� ���̷��� ��ü ID
				param[1].ParamType == ERtSctFunCmdParam::EStringAniName && //�ִϸ��̼� �̸�
				param[2].ParamType == ERtSctFunCmdParam::EIntValue && //AnimType
				param[3].ParamType == ERtSctFunCmdParam::EIntValue) //�ִϸ��̼� �ݺ� ����(0:�ѹ���,1:�ݺ�)
			{
				TArray<FString> whoIDList = GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					AActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);
					ARtObjectActor* rtObjectActor = Cast<ARtObjectActor>(actor);
					if (rtObjectActor == nullptr)
						continue;

					FString animName = param[1].ParamString;
					ERtAnimType animType = (ERtAnimType)param[2].ParamInt;
					bool isLoop = param[3].ParamInt != 0;

					FString animationPath = RtSctPath::RtAnimationPath + FString::Printf(TEXT("/%s.%s"), *animName, *animName);

					TPair<FString, float> animProgress = rtObjectActor->GetAnimationProgress();
					bool alreadyAnim = animProgress.Key == animationPath;
					if (alreadyAnim == false || !rtObjectActor->IsLoopingAnim())
					{
						if (rtObjectActor->IsSkeletalMesh())
						{
							rtObjectActor->PlayLegacyAnimation(animationPath, isLoop);
						}
						else
						{
							if (rtObjectActor->GetTctStruct())
							{
								//if (GetNetMode() != NM_Client)
								{
									if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(rtObjectActor))
									{
										npc->PlayAnim(animType, ERtSctFunCmd::PlayAnim, animName, isLoop);
									}
								}
							}
							else
								rtObjectActor->PlayAnimation(animName, isLoop);
						}
						retValue = true;
					}
					FString msg = FString::Printf(TEXT("SCT PlayAnim: %s, %s, %d : %s"), *param[0].ParamString, *animName, isLoop, *objectID);
					UE_LOG(RtGameScenarioController, Log, TEXT("%s"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);

				}
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::MoveToWayPoint(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() >= 3)
		{

			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && //��� ��ü ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID && //WayPointID
				param[2].ParamType == ERtSctFunCmdParam::EFloatValue) //MoveSpeed
			{
				FString findObjID = param[0].ParamString;
				FString findWpID = param[1].ParamString;
				float moveSpeed = param[2].Paramfloat;

				FString msg = FString::Printf(TEXT("SCT MoveToWayPoint: %s, %s, %f"), *findObjID, *findWpID, moveSpeed);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> whoIDList = GetObjectIDList(findObjID);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					AActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);
					ARtObjectActor* moveActor = Cast<ARtObjectActor>(actor);
					if (moveActor == nullptr)
						continue;
					actor = GetSceneManagerRef()->FindObjectByName(findWpID);

					ARtObjectActor* wayPoint = Cast<ARtObjectActor>(actor);
					if (wayPoint == nullptr)
						continue;

					if (FVector::Distance(moveActor->GetActorLocation(), wayPoint->GetActorLocation()) == 0.0f)
						continue;

					auto* prevData = MoveToWayPointMap.Find(moveActor);
					MoveToWayPointMap.Add(moveActor, { wayPoint, moveSpeed });
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s : Success"), *msg, *objectID);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s : %s : Success", *msg, *objectID);
					retValue = (prevData == nullptr || prevData->Key != wayPoint || prevData->Value != moveSpeed)
						&& moveActor->GetActorLocation() != wayPoint->GetActorLocation();

					// Rotate MoveActor to LookAt Waypoint : If Character Type				
					if (IsTctType(moveActor->GetActorModel()->GetRealModelNameKey(), ERtTctType::ECharacter))
					{
						FVector startLocation = moveActor->GetActorLocation();
						FVector endLocation = wayPoint->GetActorLocation();
						if (startLocation == endLocation)
							continue;
						FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(startLocation, endLocation);
						moveActor->SetActorRotation(newRotator);
					}
				}

				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::NpcMoveToWayPoint(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		/*	if (GetNetMode() == ENetMode::NM_Client)
			{
				retValue = true;
				return;
			}*/

		if (param.Num() >= 4)
		{

			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && //��� ��ü ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID && //WayPointID
				param[2].ParamType == ERtSctFunCmdParam::EStringAniName && //Anim
				param[3].ParamType == ERtSctFunCmdParam::EIntValue) //AnimType
			{
				FString findObjID = param[0].ParamString;
				FString findWpID = param[1].ParamString;
				FString animName = param[2].ParamString;
				int32 animTypeIndex = param[3].ParamInt;
				ERtAnimType animType = (ERtAnimType)animTypeIndex;

				FString msg = FString::Printf(TEXT("SCT NpcMoveToWayPoint: %s, %s, %s, %d"), *findObjID, *findWpID, *animName, animTypeIndex);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> whoIDList = GetObjectIDList(findObjID);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					AActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);
					ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(actor);
					if (npc == nullptr)
						continue;
					
					actor = GetSceneManagerRef()->FindObjectByName(findWpID);

					ARtObjectActor* wayPoint = Cast<ARtObjectActor>(actor);
					if (wayPoint == nullptr)
						continue;

					if (FVector::Distance(npc->GetActorLocation(), wayPoint->GetActorLocation()) <= 5.0f)
						continue;

					MoveToWayPointMap.Remove(npc);
					//npc->MoveToActor(wayPoint, isRun);					
					npc->MoveToTarget(wayPoint, animName, animType);

					UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s : Success"), *msg, *objectID);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s : %s : Success", *msg, *objectID);
					retValue = true;
				}

				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::StopAct(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID) // �������� ���� ������Ʈ ID
			{
				FString msg = FString::Printf(TEXT("SCT StopAct: %s"), *param[0].ParamString);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> whoIDList = GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					AActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);

					ARtObjectActor* moveActor = Cast<ARtObjectActor>(actor);
					if (moveActor == nullptr)
						continue;

					if (MoveToWayPointMap.Find(moveActor))
						retValue = true;

					MoveToWayPointMap.Remove(moveActor);

					if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(actor))
					{
						if (GetNetMode() != ENetMode::NM_Client)
							npc->SctStopAct();
						else
							npc->StopAct();

						retValue = true;
						CUSTOM_FILE_APP_LOG_EX("App.log", "%s, %s", *msg, *objectID);
						continue;
					}

					CUSTOM_FILE_APP_LOG_EX("App.log", "%s, %s", *msg, *objectID);
				}
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::AttachWeapon(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ���⸦ ������ ��� ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID) // ���� ������Ʈ ID
			{
				FString ownerID = param[0].ParamString;
				FString weaponID = param[1].ParamString;

				FString msg = FString::Printf(TEXT("SCT AttachWeapon: %s, %s"), *ownerID, *weaponID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);


				retValue = AttachWeapon(ownerID, weaponID);

				if (retValue)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				}
				else
				{
					CUSTOM_FILE_APP_LOG_EX("App.log", "SCT Attach Fail: %s, %s", *ownerID, *weaponID);
				}

				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::AttackTarget(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (GetNetMode() == ENetMode::NM_Client)
		{
			retValue = true;
			return;
		}

	if (param.Num() == 2)
	{
		if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // ������ �ϴ� NPC ID
			param[1].ParamType == ERtSctFunCmdParam::EStringID) // ������ ���� ��� ID
		{
			FString npcID = param[0].ParamString;
			FString targetID = param[1].ParamString;

			FString msg = FString::Printf(TEXT("SCT AttackTarget: %s, %s"), *npcID, *targetID);
			UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

			ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(GetSceneManagerRef()->FindObjectByName(npcID));
			if (npc == nullptr)
				return;

			TArray<FString> whoIDList = GetObjectIDList(targetID);
			if (whoIDList.IsEmpty())
			{
				npc->SctAttackTarget(nullptr);
				return;
			}

			ERtPlayerType playerType = ERtPlayerType::ENone;

			if (FRtScenarioPlayerGroup* scenarioPlayerGroup = GetScenarioPlayerGroupInfo(targetID))
				playerType = scenarioPlayerGroup->PlayerType;

			switch (playerType)
			{
			case ERtPlayerType::ENone:
			{
				ARtObjectActor* targetObj = GetSceneManagerRef()->FindObjectByName(targetID);

				npc->SctAttackTarget(targetObj);
				return;
			}
			case ERtPlayerType::ENPC:
			{
				ARtNpcObjectActor* targetObj = ARtGameSceneManager::Get(GetWorld())->GetRtNpcMap().FindRef(targetID);
				if (targetObj)
				{
					if (!targetObj->IsVisible())
						return;
					if (targetObj->GetNpcBehaviorState().ActType == ERtAnimType::ECollapse)
						return;
				}
				npc->SctAttackTarget(targetObj);
				return;
			}
			break;
			case ERtPlayerType::EPC:
			{
				if (ARtTraineePawn* traineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(targetID))
				{
					// dave
					if (!IsValid(traineePawn) || !traineePawn->IsValidLowLevel())
						return;

					npc->SctAttackTarget(traineePawn);
					return;
				}
			}
			break;
			}

			npc->SctAttackTarget(nullptr);

			if (retValue)
			{
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
			}
			else
			{
				CUSTOM_FILE_APP_LOG_EX("App.log", "SCT Attack Fail: %s, %s", *npcID, *targetID);
			}

			return;
		}
	}

	retValue = false;
}

void ARtGameScenarioController::NpcInit(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		/*	if (GetNetMode() == ENetMode::NM_Client)
			{
				retValue = true;
				return;
			}*/

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID) // ���� �ʱ�ȭ �� NPC ID
			{
				FString ownerID = param[0].ParamString;

				FString msg = FString::Printf(TEXT("SCT Npc Init: %s"), *ownerID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> whoIDList = GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
				{
					CUSTOM_FILE_APP_LOG_EX("App.log", "SCT Npc Init Fail: %s", *ownerID);
					return;
				}

				for (FString objectID : whoIDList)
				{
					AActor* actor = GetSceneManagerRef()->FindObjectByName(objectID);
					if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(actor))
					{
						MoveToWayPointMap.Remove(npc);
						npc->InitNpcState();
						UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *objectID);
					}
					else
						CUSTOM_FILE_APP_LOG_EX("App.log", "SCT Npc Init Fail", *objectID);
				}

				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::ShowScreenText(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 6)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID || // �޽����� ������ ��� ���� ID
				param[1].ParamType == ERtSctFunCmdParam::EStringMessage || // �޽���
				param[2].ParamType == ERtSctFunCmdParam::EIntValue || // ���� R
				param[3].ParamType == ERtSctFunCmdParam::EIntValue || // ���� G
				param[4].ParamType == ERtSctFunCmdParam::EIntValue || // ���� B
				param[5].ParamType == ERtSctFunCmdParam::EFloatValue) // �� �ð�(��) ��ŭ ���� �� ���� ����
			{
				FString text = param[1].ParamString;
				int r = param[2].ParamInt;
				int g = param[3].ParamInt;
				int b = param[4].ParamInt;
				float time = param[5].Paramfloat;
				FSlateColor fontColor = FSlateColor(FColor(r, g, b));

				FString msg = FString::Printf(TEXT("SCT ShowScreenText: %s, %s, %f"), *param[0].ParamString, *text, time);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> idList = GetObjectIDList(param[0].ParamString);

				if (idList.IsEmpty())
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : idList Empty"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s : idList Empty", *msg);
				}

				for (auto& objID : idList)
				{
					if (CurrentPlayerID == objID)
					{

						ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						URt_Trainee_Main* mainWidget = trainee->GetMainWidget();
						mainWidget->SetSctText(text, fontColor);

						FTimerHandle& timerHandle = trainee->MainWidget->TimerHandle;
						GetWorld()->GetTimerManager().ClearTimer(timerHandle);

						if (time > 0)
						{
							GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&, mainWidget, text]()
								{
									if (mainWidget->RemoveSctText())
									{
										UE_LOG(RtGameScenarioController, Log, TEXT("Remove Sct Text Success : %s"), *text);
									}
									else
									{
										UE_LOG(RtGameScenarioController, Log, TEXT("Remove Sct Text Fail : %s"), *text);
									}
									GetWorld()->GetTimerManager().ClearTimer(timerHandle);
								}), time, false);
						}

						UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
						CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
						retValue = false;
						return;
					}
					else
					{
						UE_LOG(RtGameScenarioController, Log, TEXT("%s : Not %s"), *msg, *objID);
						CUSTOM_FILE_APP_LOG_EX("App.log", "Fail: %s : Not %s", *msg, *objID);
					}
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::ShowTargetText(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 5)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // �޽����� ������ ��� ���� ID
				param[1].ParamType == ERtSctFunCmdParam::EStringMessage && // �޽���
				param[2].ParamType == ERtSctFunCmdParam::EFloatValue && // �� �ð�(��) ��ŭ ���� �� ���� ����
				param[3].ParamType == ERtSctFunCmdParam::EStringID, // ������ ������ Ÿ�� ID
				param[4].ParamType == ERtSctFunCmdParam::EFloatValue) // Ÿ�����κ��� ������ ��ġ
			{
				FString traineeID = param[0].ParamString;
				FString text = param[1].ParamString;
				float time = param[2].Paramfloat;
				FString targetID = param[3].ParamString;
				float height = param[4].Paramfloat;

				FString msg = FString::Printf(TEXT("SCT ShowTargetText: %s, %s, %f, %s, %f"), *traineeID, *text, time, *targetID, height);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, msg);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				TArray<FString> idList = GetObjectIDList(traineeID);

				if (idList.IsEmpty())
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : idList Empty"), *msg);

				for (auto& objID : idList)
				{
					if (CurrentPlayerID == objID)
					{

						ARtWidgetActor* widgetActor;
						URt_Trainee_Text* widget;
						FString errorMsg;

						//FindTarget
						TArray<FString> targetObjectIDList = GetObjectIDList(targetID);
						if (targetObjectIDList.IsEmpty())
						{
							UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target ID List Fail"), *msg);
							return;
						}
						FString targetObjectID = targetObjectIDList[0]; // ������ ��� ��� ��ü. 
						AActor* targetActor = GetSceneManagerRef()->FindObjectByName(targetObjectID);
						if (targetActor == nullptr)
						{
							UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target Fail"), *msg);
							return;
						}

						bool isCreate = CreateTargetTextWidgetActor(widgetActor, widget, text, targetActor, errorMsg);
						if (isCreate == false)
							UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s"), *msg, *errorMsg);

						if (widgetActor == nullptr || widget == nullptr)
							return;

						//SetTarget
						widgetActor->SetTarget(targetActor);
						widgetActor->SetWidgetRelativeLocation(0.0f, height);

						FTimerHandle& timerHandle = widgetActor->TimerHandle;
						GetWorld()->GetTimerManager().ClearTimer(timerHandle);

						if (time > 0)
						{
							GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&, widgetActor, text, targetActor]()
								{
									if (widgetActor)
									{
										widgetActor->Destroy();
										TargetTextWidgetMap.Remove({ text, targetActor });
										UE_LOG(RtGameScenarioController, Log, TEXT("TextWidget Destroy Success : %s"), *text);
									}
									else
										UE_LOG(RtGameScenarioController, Log, TEXT("TextWidget Destroy Fail : %s"), *text);
									GetWorld()->GetTimerManager().ClearTimer(timerHandle);
								}), time, false);
						}

						UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
						CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
						retValue = false;
						return;
					}
					else
					{
						UE_LOG(RtGameScenarioController, Log, TEXT("%s : Not %s"), *msg, *objID);
						CUSTOM_FILE_APP_LOG_EX("App.log", "Fail: %s : Not %s", *msg, *objID);
					}
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::ShowScreenMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringDropboxID) // �޴� ID
			{
				FString menuSetID = param[0].ParamString; // �޴� ��Ʈ ID

				FString msg = FString::Printf(TEXT("SCT ShowScreenMenu: %s"), *menuSetID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				ARtWidgetActor* widgetActor = nullptr;
				URt_Trainee_SelectionMenu* widget = nullptr;
				FString errorMsg = "";

				bool isCreate = CreateMenuWidgetActor(widgetActor, widget, menuSetID, errorMsg);
				if (isCreate == false)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s"), *msg, *errorMsg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "Fail: %s : Not %s", *msg, *errorMsg);
				}

				if (widgetActor)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
					retValue = false;
				}
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::ShowTargetMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringDropboxID && // �޴� ID
				param[1].ParamType == ERtSctFunCmdParam::EStringID) // ������ ������ Ÿ�� ID
			{
				FString menuSetID = param[0].ParamString; // �޴� ��Ʈ ID
				FString targetID = param[1].ParamString;

				FString msg = FString::Printf(TEXT("SCT ShowTargetMenu: %s, %s"), *menuSetID, *targetID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				//ARtWidgetActor* widgetActor = nullptr;
				//URt_Trainee_SelectionMenu* widget = nullptr;
				FString errorMsg = "";

				//FindTarget
				TArray<FString> targetObjectIDList = GetObjectIDList(targetID);
				if (targetObjectIDList.IsEmpty())
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target ID List Fail"), *msg);
					return;
				}
				FString targetObjectID = targetObjectIDList[0]; // ������ ��� ��� ��ü. 
				AActor* targetActor = GetSceneManagerRef()->FindObjectByName(targetObjectID);
				if (targetActor == nullptr)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target Fail"), *msg);
					return;
				}

				ARtWidgetActor_Menu* menuWidgetActor = GetWorld()->SpawnActor<ARtWidgetActor_Menu>(ARtWidgetActor_Menu::StaticClass(), FTransform());

				//bool isCreate = CreateMenuWidgetActor(widgetActor, widget, menuSetID, errorMsg);
				//if (isCreate == false) {
				if (menuWidgetActor == nullptr)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s"), *msg, *errorMsg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "Fail: %s : Not %s", *msg, *errorMsg);
				}

				//SetTarget
				if (menuWidgetActor)
				{
					menuWidgetActor->SetTarget(targetActor, RifleAimedHitLocation);
					retValue = false;
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
					CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
				}
				return;

			}
		}

	retValue = false;
}

void ARtGameScenarioController::RemoveScreenText(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		FString msg = FString::Printf(TEXT("SCT RemoveScreenText"));
	UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

	if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		trainee->GetMainWidget()->RemoveSctText();
	}

	UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
	CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);

	retValue = true;
}

void ARtGameScenarioController::RemoveTargetText(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringMessage && // �޽���
				param[1].ParamType == ERtSctFunCmdParam::EStringID) // ������ ������ Ÿ�� ID
			{
				FString text = param[0].ParamString;
				FString targetID = param[1].ParamString;

				FString msg = FString::Printf(TEXT("SCT RemoveTargetText: %s, %s"), *text, *targetID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				//FindTarget
				TArray<FString> targetObjectIDList = GetObjectIDList(targetID);
				if (targetObjectIDList.IsEmpty())
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target ID List Fail"), *msg);
					return;
				}
				FString targetObjectID = targetObjectIDList[0]; // ������ ��� ��� ��ü. 
				AActor* targetActor = GetSceneManagerRef()->FindObjectByName(targetObjectID);
				if (targetActor == nullptr)
				{
					UE_LOG(RtGameScenarioController, Log, TEXT("%s : Find Target Fail"), *msg);
					return;
				}

				RemoveTargetTextWidgetActor(text, targetActor);

				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
				retValue = false;
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::RemoveMenu(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringDropboxID) // �޴� ID
			{
				FString menuSetID = param[0].ParamString;

				FString msg = FString::Printf(TEXT("SCT RemoveMenu: %s"), *menuSetID);
				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

				RemoveMenuWidgetActor(menuSetID);


				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
				retValue = false;
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::RecordResultLog(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK
		if (param.Num() == 1)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EIntValue) // ResultLog Type
			{
				ELogProtocol resultLogType = StaticCast<ELogProtocol>(param[0].ParamInt);
				const UEnum* enumPtr = FindObject<UEnum>(NULL, TEXT("ELogProtocol"), true);

				if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
				{
					character->Get_PlayerController()->Log_Message(resultLogType);
				}

				FString msg = FString::Printf(TEXT("SCT RecordResultLog: %s"), *(enumPtr->GetNameStringByIndex(param[0].ParamInt)));

				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
				retValue = false;
				return;
			}
		}
	retValue = false;
}

void ARtGameScenarioController::PlayMovie(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		retValue = false;
}

void ARtGameScenarioController::SetTargetMenuEnabled(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringID && // MenuSetID
				param[1].ParamType == ERtSctFunCmdParam::EIntValue) // IsActive
			{
				FString menuSetID = param[0].ParamString;
				bool IsActive = param[1].ParamInt > 0;

				FString msg = FString::Printf(TEXT("SCT SetTargetMenuEnabled: %s %s"), *menuSetID, IsActive ? TEXT("True") : TEXT("False"));

				if (IsActive)
					DisabledTargetMenuList.Remove(menuSetID);
				else
					DisabledTargetMenuList.Add(menuSetID);

				if (GuideHandMap.Contains(menuSetID))
				{
					GuideHandMap.FindRef(menuSetID)->UpdateVisible();
				}

				if (ListenIconMap.Contains(menuSetID))
				{
					ListenIconMap.FindRef(menuSetID)->SetIsVisible(IsActive);
				}

				UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
				CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *msg);
				retValue = false;
				return;
			}
		}

	retValue = false;
}

void ARtGameScenarioController::SendLog(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		ENetMode netmode = GetNetMode();
	if (netmode == ENetMode::NM_Client && param.Num() == 1)
	{
		if (param[0].ParamType == ERtSctFunCmdParam::EIntValue)
		{
			ERtSctSendLogType sendLogType = StaticCast<ERtSctSendLogType>(param[0].ParamInt);

			ELogProtocol* logProtocol = SendLogMap.Find(sendLogType);
			if (logProtocol) {
				if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
				{
					ARtPlayerController* rtPlayerController = character->Get_PlayerController();
					rtPlayerController->Log_Message(*logProtocol);
				}
			}

			retValue = false;
			return;
		}
	}

	retValue = false;
}

void ARtGameScenarioController::IsNotValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->VarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					retValue = (*value != param[1].ParamInt);
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::IsGreaterEqualValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->VarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					retValue = (*value >= param[1].ParamInt);
					return;
				}
			}
		}

	retValue = false;
}

void ARtGameScenarioController::IsLessEqualValue(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 2)
		{
			if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
				param[1].ParamType == ERtSctFunCmdParam::EIntValue)
			{
				auto* value = ScenarioModel->VarialbleMap.Find(param[0].ParamString);
				if (value)
				{
					retValue = (*value <= param[1].ParamInt);
					return;
				}
			}
		}

	retValue = false;
}

// ������ �޴� �׸� �� ���� ����/�Ⱥ��� ó���� �Ѵ�.
void ARtGameScenarioController::SetVisibleRadioMenuItem(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (ERt_PlayType::E_Trainee == CurrentPlayType)
		{
			if (param.Num() == 2)
			{
				if (param[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
					param[1].ParamType == ERtSctFunCmdParam::EIntValue)
				{
					for (auto& item : ScenarioModel->RadioDropboxCommand.DropboxElemantList)
					{
						if (item.EventID == param[0].ParamString) {
							item.bVisible = (param[1].ParamInt == 1);

							// ������ �޴� ����Ʈ ������Ʈ
							if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
							{
								trainee->UpdateWalkieTalkieMenuList();
							}
							return;
						}
					}
				}
			}
		}

	retValue = false; 
}

void ARtGameScenarioController::SendPacketToScenarioServerForEnd(int32 TID)
{
	// Send Packet 'End'
	auto* netmgr = Get_NetManager();
	if (netmgr) {
		netmgr->SendPacket_TrainingEnd(TID);
	}

	auto* WebSubsys = Get_WebManager();
	if (WebSubsys->IsInitialized())
	{
		if (auto* gs = GetGameState()) {
			auto info = gs->Get_Scenario_Info();

			FString InId;
			InId = info.Destination;
			WebSubsys->CheckTID(gs->Training_Index);
		}
	}
}

// �ó������󿡼� ������ ������ �������� ���� ��ȣ�� �� �� ������. 
// �������� ����� ���� �����ȣ�� ������ ó���� �ƴϴ�. �װ��� ���� ����.
void ARtGameScenarioController::EndTraining(TArray<FRtSctFunctionCommandParam> param, bool& retValue)
{
	SCRIPT_FUNCTION_CHECK

		if (param.Num() == 0) {
			if (CurrentPlayType == ERt_PlayType::E_Control) {
				// �Ÿ� �α׸� ���� ������.
				ARtReplayManagerActor::Get(GetWorld())->SendAllPlayerDistanceLog();
				ARtGameState* gameState = GetGameState();
				if (gameState) {
					// ���� �߰� �Ǹ� ����.
					int32 TID = gameState->Training_Index;
					SendPacketToScenarioServerForEnd(TID);
				}
			}
		}

	retValue = false;
}

// �� �Լ��� Ư���� ó���� �߻��Ǵ� ������ �� ���� ���� �Ǿ�� �Ͽ� ���� ���� ó�� �Լ��� �ٸ��� ó���Ѵ�. 
// ���� ó�� �Լ��� ������ ������ ���� �� �� ��ε��ɽ�Ʈ �Ͽ� �޾� ó�� �ϴ� ���� �̴�.
bool ARtGameScenarioController::Force_RequestRandomNum(const TArray<FRtSctFunctionCommand>* commandArray)
{
	if (!commandArray)
		return false;

	if (ERt_PlayType::E_Trainee == CurrentPlayType)
	{
		for (const FRtSctFunctionCommand& command : *commandArray)
		{
			const ERtSctFunCmd& cmd = command.FunctionCommand;

			if (cmd == ERtSctFunCmd::RequestRandomNum)
			{
				if (command.Params.Num() == 2)
				{
					if (command.Params[0].ParamType == ERtSctFunCmdParam::EStringVariable &&
						command.Params[1].ParamType == ERtSctFunCmdParam::EIntValue)
					{
						auto* value = ScenarioModel->VarialbleMap.Find(command.Params[0].ParamString);
						if (value) {

							if (ARtPlayerController* PlayerController = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
							{
								PlayerController->Server_RequestRandomNum(command.Params[0].ParamString, command.Params[1].ParamInt);
#if WITH_EDITOR
								FString playType =
									URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)CurrentPlayType);

								APP_LOG("1. NetworkTest ([Client] -> Server): Server_RequestRandomNum, %s", *playType);
#endif
								return true;
							}
							//							Server_RequestRandomNum(param[0].ParamString, param[1].ParamInt);
						}
					}
				}
			}
		}
	}

	return false;
}

// Sct Ŀ�ǵ� ���� ��ü �̸� ID�� �ؼ��Ͽ� ������ ��ü �̸� ����Ʈ�� �����Ѵ�.
TArray<FString> ARtGameScenarioController::GetObjectIDList(FString objectID)
{
	TArray<FString> objectIDList;


	FString linkIDString = TEXT("");

	//if(CurrentSctEventCommand)
	//{
	//	if (objectID == TEXT("{TargetID}"))
	//	{
	//		linkIDString = CurrentSctEventCommand->TargetObjectID;
	//	}
	//	else if (objectID == TEXT("{OwnerID}"))
	//	{
	//		linkIDString = CurrentSctEventCommand->OwnerObjectID;
	//	}
	//}

	//if (linkIDString.IsEmpty())
	//{
		//�׷� ���� ���
	for (auto& playerGroupItem : ScenarioModel->SctData.PlayerGroupList)
	{
		if (playerGroupItem.PlayerGroupName == objectID)
		{
			for (auto& playerItem : playerGroupItem.PlayerList)
			{
				objectIDList.Add(playerItem.PlayerObjectID);
			}
			break;
		}
	}

	if (objectIDList.IsEmpty())
		objectIDList.Add(objectID); // ������ ���
	//}
	//else if (linkIDString == TEXT("{PcGroups}"))
	//{
	//	for (auto& playerGroupItem : ScenarioModel->SctData.PlayerGroupList)
	//	{
	//		for (auto& playerItem : playerGroupItem.PlayerList)
	//		{
	//			objectIDList.Add(playerItem.PlayerObjectID);
	//		}
	//	}
	//}
	//else
	//{
	//	FString playerGroupID;
	//	int32 groupType = 0;
	//
	//	// �׷� ��ü ���̵� ���� Ȯ���Ѵ�.
	//	if (INDEX_NONE != linkIDString.Find(TEXT("G:")))
	//	{
	//		playerGroupID = linkIDString.Replace(TEXT("G:"), TEXT(""));
	//		groupType = 1;
	//	}
	//	// �׷� ���� ���̵� ���� Ȯ���Ѵ�.
	//	else if (INDEX_NONE != linkIDString.Find(TEXT("GL:")))
	//	{
	//		playerGroupID = linkIDString.Replace(TEXT("GL:"), TEXT(""));
	//		groupType = 2;
	//	}
	//	// �׷� �� ���̵� ���� Ȯ���Ѵ�.
	//	else if (INDEX_NONE != linkIDString.Find(TEXT("GE:")))
	//	{
	//		playerGroupID = linkIDString.Replace(TEXT("GE:"), TEXT(""));
	//		groupType = 3;
	//	}
	//	else
	//	{
	//		objectIDList.Add(linkIDString);
	//	}
	//
	//	if (groupType > 0)
	//	{
	//		for (auto& playerGroupItem : ScenarioModel->SctData.PlayerGroupList)
	//		{
	//			if (playerGroupItem.PlayerGroupID == playerGroupID)
	//			{
	//				for (auto& playerItem : playerGroupItem.PlayerList)
	//				{
	//					if (groupType == 1)
	//					{
	//						objectIDList.Add(playerItem.PlayerObjectID);
	//					}
	//					else if (groupType == 2)
	//					{
	//						if (playerItem.IsGroupLeader)
	//						{
	//							objectIDList.Add(playerItem.PlayerObjectID);
	//							break;
	//						}
	//					}
	//					else if (groupType == 3)
	//					{
	//						if (!playerItem.IsGroupLeader)
	//						{
	//							objectIDList.Add(playerItem.PlayerObjectID);
	//						}
	//					}
	//				}
	//
	//				break;
	//			}
	//		}
	//	}
	//
	//}

	return objectIDList;
}

// �������� �̺�Ʈ ������ ��������
FRtScenarioEvent* ARtGameScenarioController::GetScenarioEvent(int32 index)
{
	if (ScenarioModel.IsValid())
	{
		if (0 <= index && index < ScenarioModel->SctData.EventList.Num())
		{
			return &ScenarioModel->SctData.EventList[index];
		}
	}

	return nullptr;
}

FRtScenarioPlayerGroup* ARtGameScenarioController::GetScenarioPlayerGroupInfo(FString playerObjectID)
{
	if (ScenarioModel.IsValid())
	{
		for (auto& group : ScenarioModel->SctData.PlayerGroupList)
		{
			for (auto& player : group.PlayerList)
			{
				if (player.PlayerObjectID == playerObjectID)
				{
					return &group;
				}
			}
		}
	}

	return nullptr;
}

// �÷��̾� ID �� �÷��̾� ���� ������ ��������
FRtScenarioPlayer* ARtGameScenarioController::GetScenarioPlayerInfo(FString playerObjectID)
{
	if (ScenarioModel.IsValid())
	{
		for (auto& group : ScenarioModel->SctData.PlayerGroupList)
		{
			for (auto& player : group.PlayerList)
			{
				if (player.PlayerObjectID == playerObjectID)
				{
					return &player;
				}
			}
		}
	}

	return nullptr;
}

FString ARtGameScenarioController::GetModelPathByObjectID(FString playerObjectID)
{
	if (GetSceneManagerRef())
	{
		//bool bIsSuccess = false;
		// FRtTctData tctData = GetSceneManagerRef()->GetTctDataByObjectName(playerObjectID, bIsSuccess);
		FRtTctStruct tctStruct;
		bool bIsSuccess = GetSceneManagerRef()->GetTctDataByObjectNameInData(playerObjectID, tctStruct);

		if (bIsSuccess)
		{
			return tctStruct.ComDefault.ModelFileName;
			//			return tctData.ComObject.ModelFilePath;
		}
	}

	return FString();
}

bool ARtGameScenarioController::IsTctType(FString modleKeyName, ERtTctType inputTctType)
{
	bool isGetTctData = false;
	FRtTctData data = GetSceneManagerRef()->GetTctDataByModelKey(modleKeyName, isGetTctData);
	if (isGetTctData == false)
		return false;

	FRtComObject comObject = data.ComObject;
	return comObject.TctType == inputTctType;
}

bool ARtGameScenarioController::AttachWeapon(FString characterId, FString equipmentId)
{
	TArray<FString> whoIDList = GetObjectIDList(characterId);
	if (whoIDList.IsEmpty())
		return false;

	ERtPlayerType playerType = ERtPlayerType::ENone;

	if (FRtScenarioPlayerGroup* scenarioPlayerGroup = GetScenarioPlayerGroupInfo(characterId))
		playerType = scenarioPlayerGroup->PlayerType;

	switch (playerType)
	{
	case ERtPlayerType::EPC:
	{
		ARtTraineePawn* traineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(whoIDList[0]);

		// dave
		if (!IsValid(traineePawn) || !traineePawn->IsValidLowLevel())
			return false;

		if (traineePawn == nullptr)
			return false;

		AActor* findWeaponActor = GetSceneManagerRef()->FindObjectByName(equipmentId);
		ARtObjectActor* weaponActor = Cast<ARtObjectActor>(findWeaponActor);
		return traineePawn->AttachWeapon(weaponActor);
	}
	break;
	case ERtPlayerType::ENPC:
	{
		AActor* actor = GetSceneManagerRef()->FindObjectByName(whoIDList[0]);
		ARtObjectActor* npcActor = Cast<ARtObjectActor>(actor);
		if (npcActor == nullptr)
			return false;

		actor = GetSceneManagerRef()->FindObjectByName(equipmentId);
		ARtObjectActor* weaponActor = Cast<ARtObjectActor>(actor);
		if (weaponActor == nullptr)
			return false;

		weaponActor->AttachToNpc(npcActor);
		return true;
	}
	break;
	}

	return false;
}

// ���� �÷��̾� ID(�ڱ� �ڽ�) ����. 
// �÷��̾� ���� ������ ����.
void ARtGameScenarioController::SetCurrentPlayerID(FString playerID)
{
	CurrentPlayerID = playerID;

	if (!CurrentPlayerID.IsEmpty())
	{
		FRtScenarioPlayer* sctPlayerInfo = GetScenarioPlayerInfo(CurrentPlayerID);
		if (sctPlayerInfo)
		{
			MyScenarioPlayer = *(sctPlayerInfo);
		}
	}
}

void ARtGameScenarioController::SetGamePlay(bool isGamePlay)
{
	ENetMode netmode = GetNetMode();
	FString netmodeString;
	if (ENetMode::NM_Client == netmode) {
		netmodeString = TEXT("NM_Client");
	}
	else if (ENetMode::NM_ListenServer == netmode)
	{
		netmodeString = TEXT("NM_ListenServer");
	}

	CUSTOM_FILE_APP_LOG_EX("App.log", "%s ( %s ) : %s Training",
		*URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)CurrentPlayType),
		*netmodeString,
		isGamePlay ? TEXT("Start") : TEXT("End"));

	bIsGamePlay = isGamePlay;

	RifleAimedObjectKeyName = TEXT("");

	//if (SceneManagerRef.IsValid())
	//{
	//	GetSceneManagerRef()->ObjectAllVisibleByTctType(!bIsGamePlay, ERtTctType::EPlayerSpawn);
	//}

	ARtReplayManagerActor::Get(GetWorld())->InitReplay();
	if (isGamePlay)
	{
		if (ScenarioModel.IsValid())
		{
			ScenarioModel->GameState = ERtSctGameState::EGameState_Play;
		}

		if (CurrentPlayType == ERt_PlayType::E_Control) /* || CurrentPlayType == ERt_PlayType::E_Single*/
		{
			ARtReplayManagerActor::Get(GetWorld())->StartRecord(); // ���� ����.
		}

		AllNpcPosTimer = 0;
		TrainingTimer = 0;
	}

	if (SetSctGamePlay.IsBound())
		SetSctGamePlay.Broadcast(isGamePlay);

}

void ARtGameScenarioController::InitScenario()
{
	if (bIsGamePlay)
		SetGamePlay(false);

	if (!ScenarioModel.IsValid())
		return;

	// ������ �ʱ�ȭ
	ScenarioModel->EventIDLoopList = ScenarioModel->InitEventIDLoopList;
	ScenarioModel->VarialbleMap = ScenarioModel->InitVarialbleMap;
	ScenarioModel->OldVarialbleMap = ScenarioModel->InitVarialbleMap;

	RifleAimedObjectKeyName.Empty(); // ������ �ܴ� �� Ȯ�εǴ� ��ü�� Ű �̸�.
	RifleShotObjectKeyName.Empty(); // ������ �� ���� ��ü�� Ű �̸�.
	AimedTarget = nullptr; // ������ �ܴ� ��ü ����

	MoveToWayPointMap.Empty();

	if (GetMainWidget())
		GetMainWidget()->RemoveSctText();

	for (auto p : TargetTextWidgetMap)
		p.Value->DestroyWidgetActor();
	TargetTextWidgetMap.Empty();

	if (MenuWidget.Get())
	{
		if (ARtWidgetActor_Menu* handMenuWidget = Cast<ARtWidgetActor_Menu>(MenuWidget))
			handMenuWidget->Clear();
	}
	MenuWidget = nullptr;

	Resv_AimMap.Empty(); // PlayerID, ObjectID
	Process_NewAimMap.Empty(); // PlayerID, ObjectID
	Process_AimingMap.Empty(); // PlayerID, ObjectID
	IsChangeAim = false;

	Resv_InteractKeyList.Empty(); // PlayerID
	Process_InteractKeyList.Empty(); // PlayerID

	Resv_ShotList.Empty();
	Process_ShotList.Empty();

	DisabledTargetMenuList.Empty();

	// ���� �ʱ�ȭ
	if (ARtGameSceneManager* gameSceneManager = Cast<ARtGameSceneManager>(GetSceneManagerRef()))
	{
		for (TPair<ARtObjectActor*, ARtGameSceneManager::FInitObjectData>& datas : gameSceneManager->GetInitObjectDatas())
		{
			ARtObjectActor* actor = datas.Key;
			ARtGameSceneManager::FInitObjectData data = datas.Value;

			data.InitData(actor);
		}
	}

	if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		trainee->Init();
	}

	// ��� Listen Icon Ȱ��ȭ
	for (const TPair<FString, ARtWidgetActor_ListenIcon*>& iconMap : ListenIconMap)
	{
		const FString& Key = iconMap.Key;
		ARtWidgetActor_ListenIcon* Icon = iconMap.Value;

		if (Icon && !Icon->IsPendingKill())
		{
			Icon->SetIsVisible(true);

			URtListenIconUserWidget* iconWidget = Cast<URtListenIconUserWidget>(Icon->GetWidgetComponent()->GetUserWidgetObject());

			if (iconWidget)
			{
				iconWidget->SetIsListening(false);
			}
		}
	}

	if (ActivatedListenIconActor)
	{
		ActivatedListenIconActor = nullptr;
	}

	TrainingTimer = 0;
}

void ARtGameScenarioController::LogOut()
{
	//if (URtNetworkSubsystem::Get())
	//{
	//	SetGamePlay(false);
	//	URtNetworkSubsystem::Get()->Disconnect();
	//
	//	if (nullptr != ScenarioModel)
	//	{
	//		ScenarioModel->GameState = ERtSctGameState::EGameState_None;
	//		ScenarioModel->EventIDLoopList = BackUp_EventIDLoopList;
	//	}
	//
	//	CurrentPlayerID = TEXT("");
	//	bIsGamePlay = false;
	//}
}

URtNetworkManager* ARtGameScenarioController::Get_NetManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtNetworkManager>();

	return nullptr;
}

URtWebManager* ARtGameScenarioController::Get_WebManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtWebManager>();

	return nullptr;
}

void ARtGameScenarioController::ExecuteServerEvent(const FString& eventID)
{
	ARtPlayerState* myPlayerState = GetMyPlayerState();
	if (myPlayerState) {
		FString playerInfo = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)myPlayerState->Play_Type);
		if (myPlayerState->Play_Type == ERt_PlayType::E_Trainee)
		{
			playerInfo = myPlayerState->Login_DeviceName;
		}
		FILE_APP_LOG("App.log", "[%s][SCT ServerEvent: %s]", *playerInfo, *eventID);
	}

	// dave
	if (!ScenarioModel.IsValid() || !ScenarioModel->IsValidLowLevel())
		return;

	if (ScenarioModel == nullptr)
		return;

	if (CurrentPlayType == ERt_PlayType::E_Single)
	{
		if (!bIsGamePlay)
			return;
	}

	const FRtEventCommand* command = ScenarioModel->FindEventCommand(eventID);
	if (command)
		ExecuteProcessCommands(command, true);

	TPair<const FRtEventCommand*, FString> data = ScenarioModel->ServerEventCommandMap.FindRef(eventID);
	if (data.Key == nullptr)
		return;

	const FRtEventCommand* commands = data.Key;
	const FString menuSetID = data.Value;


	if (!menuSetID.IsEmpty())
		RemoveMenuWidgetActor(menuSetID);

	if (DebugMode)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ExecuteServerEvent : %s"), *eventID));
}

void ARtGameScenarioController::LineTraceAim()
{
	ENetMode netmode = GetNetMode();
	if (ENetMode::NM_Client != netmode && ENetMode::NM_Standalone != netmode)
		return;

	FVector shotStart = FVector::ZeroVector;
	if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		shotStart = character->ShotStart->GetComponentLocation();
		FVector WorldDirection = shotStart + character->ShotStart->GetForwardVector() * 100000.0f;

		FHitResult hitResult;
		TArray<AActor*> actorsToIgnore = { character };
		EDrawDebugTrace::Type debugLineType = DebugMode ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		bool isHit = UKismetSystemLibrary::LineTraceSingleForObjects(character->GetWorld(), shotStart, WorldDirection,
			ObjectTypes, false, actorsToIgnore, debugLineType, hitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

		AActor* targetActor = NULL;
		if (isHit) // gltf skeletalmesh actor�� ��� �θ��� RtObjectActor�� ã�� ���� ����ó��
		{
			targetActor = hitResult.GetActor();

			if (targetActor->GetClass() == AglTFRuntimeFBXAssetActor::StaticClass())
				targetActor = targetActor->GetAttachParentActor();

			if (!targetActor)
				isHit = false;
		}

		if (isHit)
		{
			RifleAimedHitLocation = hitResult.Location;
			character->ShotHitDecal->SetWorldLocation(RifleAimedHitLocation);

			URtObjectOptionComponent* pObjectOption =
				Cast<URtObjectOptionComponent>(targetActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));

			FString newObjectKeyName = TEXT("");

			FString logStr;
			if (pObjectOption)
			{
				newObjectKeyName = pObjectOption->GetObjectKeyName();
				logStr = FString::Printf(TEXT("Aim: %s [%s]"), *newObjectKeyName, *targetActor->GetName());
			}
			else if (targetActor)
				logStr = FString::Printf(TEXT("Aim: None [%s]"), *targetActor->GetName());
			else
				logStr = FString::Printf(TEXT("Aim: None"));

			const uint64 screenKey = GetTypeHash(FName("Aim"));
			GEngine->AddOnScreenDebugMessage(screenKey, 0.1f, FColor::Emerald, logStr);

			//			FILE_APP_LOG("shipping_debug.log", "%s", *logStr);

			if (newObjectKeyName != RifleAimedObjectKeyName)
			{
				RifleAimedObjectKeyName = newObjectKeyName;
				AimedTarget = Cast<ARtObjectActor>(targetActor);
				if (ARtPlayerController* rtPlayerController = character->Get_PlayerController())
				{
					FRtDeviceInfo currinfo;
					if (rtPlayerController->Get_DeviceInfo(currinfo))
					{
						int32 UID = currinfo.PlayerInfo.UID;
						if (CurrentPlayType != ERt_PlayType::E_Single)
						{
							if (auto* nmgr = Get_NetManager())
							{
								nmgr->SendPacket_TraineeAim(UID, RifleAimedObjectKeyName);
							}
						}
						else
							AddAimMap(UID, RifleAimedObjectKeyName);
					}
				}
			}
		}
		else
		{
			if (RifleAimedObjectKeyName != TEXT(""))
			{
				RifleAimedObjectKeyName = TEXT("");
				AimedTarget = nullptr;

				ARtPlayerController* rtPlayerController = character->Get_PlayerController();

				FRtDeviceInfo currinfo;
				if (rtPlayerController->Get_DeviceInfo(currinfo))
				{
					int32 UID = currinfo.PlayerInfo.UID;
					if (CurrentPlayType != ERt_PlayType::E_Single)
					{
						if (auto* nmgr = Get_NetManager())
						{
							nmgr->SendPacket_TraineeAim(UID, RifleAimedObjectKeyName);
						}
					}
					else
						AddAimMap(UID, RifleAimedObjectKeyName);
				}
			}

			RifleAimedHitLocation = WorldDirection;
		}

		if (IsGuideAim)
		{
			if (AimLine)
			{
				// ���� ����Ʈ ��ġ ����
				FVector NewStartPoint = shotStart;
				FVector NewEndPoint = RifleAimedHitLocation;
				AimLine->SetLocationAtSplinePoint(0, NewStartPoint, ESplineCoordinateSpace::World);
				AimLine->SetLocationAtSplinePoint(1, NewEndPoint, ESplineCoordinateSpace::World);

				// ���ö��� ������Ʈ
				AimLine->UpdateSpline();

				// ���ö��� �޽� ������Ʈ
				FVector StartPoint, StartTangent, EndPoint, EndTangent;
				AimLine->GetLocationAndTangentAtSplinePoint(0, StartPoint, StartTangent, ESplineCoordinateSpace::World);
				AimLine->GetLocationAndTangentAtSplinePoint(1, EndPoint, EndTangent, ESplineCoordinateSpace::World);

				AimLineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
			}
			else
			{
				// SplineComponent �ʱ�ȭ
				AimLine = NewObject<USplineComponent>(this, TEXT("AimLine"));
				AimLine->SetupAttachment(RootComponent);
				AimLine->RegisterComponent();

				FVector StartPoint = shotStart;
				FVector EndPoint = RifleAimedHitLocation;
				FVector StartTangent, EndTangent;

				AimLine->SetLocationAtSplinePoint(0, StartPoint, ESplineCoordinateSpace::World);
				AimLine->SetLocationAtSplinePoint(1, EndPoint, ESplineCoordinateSpace::World);
				AimLine->UpdateSpline();

				// Spline Mesh ����
				AimLineMesh = NewObject<USplineMeshComponent>(this);
				AimLineMesh->SetMobility(EComponentMobility::Movable);
				AimLineMesh->AttachToComponent(AimLine, FAttachmentTransformRules::KeepRelativeTransform);

				AimLine->GetLocationAndTangentAtSplinePoint(0, StartPoint, StartTangent, ESplineCoordinateSpace::World);
				AimLine->GetLocationAndTangentAtSplinePoint(1, EndPoint, EndTangent, ESplineCoordinateSpace::World);

				AimLineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
				if (UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/RS/Training/Blueprints/SM_Aim.SM_Aim")))
					AimLineMesh->SetStaticMesh(StaticMesh);

				AimLineMesh->RegisterComponent();
			}
		}
	}
}

void ARtGameScenarioController::Interact_AimTarget()
{
	if (ScenarioModel == nullptr || HideMenuWidget())
		return;

	if (AimedTarget.IsValid()) // ������ Ÿ���� ���� ��
	{
		if (OpenDropbox(AimedTarget, RifleAimedHitLocation))
			return;

		{
			//// BŰ�� Ÿ�� �޴� ����
			//bool isMenu = (ScenarioModel->TargetDropboxMap.IsEmpty() == false)
			//	&& ScenarioModel->TargetDropboxMap.Contains(RifleAimedObjectKeyName);
			//FString menuSetID;
			//if (isMenu)
			//{
			//	menuSetID = *(ScenarioModel->TargetDropboxMap.FindRef(RifleAimedObjectKeyName));
			//	isMenu = IsTargetMenuEnable(menuSetID);
			//}
			//if (isMenu)
			//{
			//	// Ÿ�� �޴� ����
			//	if (ARtObjectActor* target = AimedTarget.Get())
			//	{
			//		FString msg = FString::Printf(TEXT("SCT ShowTargetMenu %s %s"), *menuSetID, *RifleAimedObjectKeyName);
			//		UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

			//		FString errorMsg = "";

			//		ARtWidgetActor_Menu* menuWidgetActor = GetTargetMenuWidget();

			//		if (menuWidgetActor == nullptr)
			//			UE_LOG(RtGameScenarioController, Log, TEXT("%s : %s"), *msg, *errorMsg);

			//		//SetTarget
			//		if (menuWidgetActor)
			//		{
			//			//Get Menu Commands
			//			FRtDropboxCommand* dropboxCommand = ScenarioModel->DropboxCommandMap.Find(menuSetID);
			//			if (dropboxCommand == nullptr)
			//			{
			//				errorMsg = "Dropbox ID is Not Found";
			//				menuWidgetActor->Clear();
			//				return;
			//			}
			//			if (dropboxCommand->DropboxElemantList.IsEmpty())
			//			{
			//				errorMsg = "DropboxElemantList is Empty";
			//				menuWidgetActor->Clear();
			//				return;
			//			}

			//			MenuWidget = menuWidgetActor;
			//			menuWidgetActor->SetTarget(target, RifleAimedHitLocation);

			//			menuWidgetActor->Init(dropboxCommand->DropboxElemantList.Num());

			//			if (GuideHandMap.Contains(menuSetID))
			//			{
			//				ARtGuideHand* guideHand = GuideHandMap.FindRef(menuSetID);
			//				menuWidgetActor->GuideHand = guideHand;
			//				guideHand->IsGraped = true;
			//				guideHand->UpdateVisible();
			//			}

			//			//Set Menu Commands
			//			int32 index = 0;
			//			for (auto& data : dropboxCommand->DropboxElemantList)
			//			{
			//				// data.EventIndex
			//				menuWidgetActor->CreateMenuOption(index++, data.EventID, data.EventID);
			//			}

			//			UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
			//		}
			//	}
			//	return;
			//}
		}

		// Ÿ�� ��ȣ�ۿ� �̺�Ʈ�� �ִ°�?
		if (ScenarioModel->TargetInteractObjects.Contains(RifleAimedObjectKeyName))
		{
			Interact_TargetEvent();
			return;
		}
	}

	// �ٲ� ������
	if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		character->ToggleHideWalkieTalkie();
	}

	// �� �� ������ ������ (SoldierRadio)
	//FString msg = FString::Printf(TEXT("SCT Soldier Radio"));
	//UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

	//// ���� ������ �޴�
	//if (ScenarioModel->RadioDropboxCommand.DropboxElemantList.IsEmpty() == false)
	//{
	//	ARtTraineePawn* traineePawn = Cast<ARtTraineePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//	FVector start = traineePawn->Camera->GetComponentLocation() - 50 * FVector::UpVector;
	//	FVector direction = traineePawn->IsVR ? traineePawn->SkeletalMesh->GetForwardVector() : traineePawn->SkeletalMesh->GetRightVector();
	//	FVector end = start + direction * 50.0f;
	//	FRotator rot = UKismetMathLibrary::FindLookAtRotation(start, end);

	//	Open_DefaultMenu(end, rot);
	//}
	//else
	//{
	//	UE_LOG(RtGameScenarioController, Log, TEXT("%s : No Soldier Radio"), *msg);
	//	return;
	//}

	//if (MenuWidget.Get())
	//	UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);
}

void ARtGameScenarioController::Open_DefaultMenu(const FVector& loc, const FRotator& rot)
{
	MenuWidget = GetRadioWidget();
	if (NULL == MenuWidget)
		return;

	MenuWidget->SetActorLocation(loc);
	MenuWidget->SetActorRotation(rot);

	GetRadioWidget()->TryOpen();
}

void ARtGameScenarioController::Interact_Target(TWeakObjectPtr<ARtObjectActor> target, USphereComponent* handCollision)
{
	if (ScenarioModel == nullptr || HideMenuWidget())
		return;

	if (target.IsValid())
	{
		FString objectName = target.Get()->GetObjectName();
		if (objectName.IsEmpty())
			return;

		// Dropbox
		if (handCollision && OpenDropbox(target, handCollision->GetComponentLocation()))
			return;

		// Ÿ�� ��ȣ�ۿ� �̺�Ʈ�� �ִ°�?	
		if (ScenarioModel->TargetInteractObjects.Contains(objectName))
		{
			Interact_TargetEvent();
			return;
		}
	}

	// �� �� ������ ������ (SoldierRadio)

	FString msg = FString::Printf(TEXT("SCT Soldier Radio"));
	UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);

	// ���� ������ �޴�
	if (ScenarioModel->RadioDropboxCommand.DropboxElemantList.IsEmpty() == false)
	{
		APlayerCameraManager* cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector cameraLocCalc = cameraManager->GetCameraLocation();

		Open_DefaultMenu(handCollision->GetComponentLocation(), UKismetMathLibrary::FindLookAtRotation(cameraLocCalc, handCollision->GetComponentLocation()));
	}
	else
	{
		UE_LOG(RtGameScenarioController, Log, TEXT("%s : No Soldier Radio"), *msg);
		return;
	}

	if (MenuWidget.Get())
		UE_LOG(RtGameScenarioController, Log, TEXT("%s : Success"), *msg);

	return;
}

void ARtGameScenarioController::Interact_TargetEvent()
{
	if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		ARtPlayerController* rtPlayerController = character->Get_PlayerController();
		FRtDeviceInfo currinfo;
		if (rtPlayerController->Get_DeviceInfo(currinfo))
		{
			int32 UID = currinfo.PlayerInfo.UID;
			if (CurrentPlayType != ERt_PlayType::E_Single)
			{
				if (auto* nmgr = Get_NetManager())
				{
					nmgr->SendPacket_InteractKey(UID);
				}
			}
			else
				AddInteractKeyList(UID);
		}
	}
}

bool ARtGameScenarioController::OpenDropbox(TWeakObjectPtr<ARtObjectActor>& target, FVector Location)
{
	FString objectName = target->GetObjectName();

	// Ÿ�� �޴� ������ ����
	if (ScenarioModel->TargetDropboxMap.IsEmpty() ||
		!ScenarioModel->TargetDropboxMap.Contains(objectName))
	{
		UE_LOG(RtGameScenarioController, Error, TEXT("Open Dropbox Failed : No Dropbox"));
		return false;
	}

	ARtWidgetActor_Menu* menuWidgetActor = GetTargetMenuWidget();

	if (menuWidgetActor == nullptr)
	{
		UE_LOG(RtGameScenarioController, Error, TEXT("Open Dropbox Failed : No MenuWidget Actor"));
		return false;
	}

	bool isMenu = false;
	const TArray<FString>& menuSetIDList = ScenarioModel->TargetDropboxMap.FindRef(objectName);

	for (FString menuID : menuSetIDList)
	{
		isMenu = IsTargetMenuEnable(menuID);

		if (!isMenu)
			continue;

		//Get Menu Commands
		FRtDropboxCommand* dropboxCommand = ScenarioModel->DropboxCommandMap.Find(menuID);
		if (dropboxCommand == nullptr || dropboxCommand->DropboxElemantList.IsEmpty())
		{
			UE_LOG(RtGameScenarioController, Warning, TEXT("Open Dropbox Warning : %s Dropbox Commands Empty"), *menuID);
			continue;
		}

		//SetTarget
		MenuWidget = menuWidgetActor;
		menuWidgetActor->SetTarget(target.Get(), Location);
		menuWidgetActor->Init(dropboxCommand->DropboxElemantList.Num());

		if (GuideHandMap.Contains(menuID))
		{
			ARtGuideHand* guideHand = GuideHandMap.FindRef(menuID);
			menuWidgetActor->GuideHand = guideHand;
			guideHand->IsGraped = true;
			guideHand->UpdateVisible();
		}

		//Set Menu Commands
		int32 index = 0;
		for (auto& data : dropboxCommand->DropboxElemantList)
		{
			//data.EventIndex
			menuWidgetActor->CreateMenuOption(index++, data.EventID, data.EventID);
		}

		// Ÿ�� �޴� ����
		FString msg = FString::Printf(TEXT("SCT ShowTargetMenu %s %s"), *menuID, *objectName);
		UE_LOG(RtGameScenarioController, Log, TEXT("%s : Attempt"), *msg);
		return true;
	}

	menuWidgetActor->Clear();
	return false;
}

void ARtGameScenarioController::CloseMenu()
{
	if (ARtWidgetActor_Menu* menuWidget = Cast<ARtWidgetActor_Menu>(MenuWidget))
	{
		
		const ARtWidgetActor_MenuOption* HoverOption = menuWidget->HoverOption.Get();
		// ������
		if (MenuWidget.Get() == GetRadioWidget())
		{
			GetRadioWidget()->TryClose();
			return;
		}

		// Ÿ�� �޴�
		HideMenuWidget(true);
		return;

	}
}

bool ARtGameScenarioController::HideMenuWidget(bool IsImmediate)
{
	//if (GetMainWidget())
	//	if (GetMainWidget()->HideMenuWidget())
	//		return true;

	if (MenuWidget.Get())
	{
		if (MenuWidget.Get() == GetRadioWidget())
		{
			switch (GetRadioWidget()->RadioState)
			{
			case ESoldierRadioState::ETryOpen:
			case ESoldierRadioState::ETryClose:
				if (!IsImmediate)
				{
					GetRadioWidget()->Cancle();
					return true;
				}
			case ESoldierRadioState::ESelect:
			case ESoldierRadioState::ECommunicationSuccess:
			case ESoldierRadioState::ECommunicationFail:
				GetRadioWidget()->TryClose(IsImmediate);
				if (IsImmediate)
					MenuWidget = nullptr;
				return true;
			}

		}
		else if (MenuWidget.Get() == GetTargetMenuWidget())
		{
			MenuWidget->Clear();
			MenuWidget = nullptr;
			return true;
		}
	}
	return false;
}

void ARtGameScenarioController::UpdateListeningNpc(const FString& CurNpcName)
{
	// ���� Ȱ��ȭ�� Icon�� ��Ȱ��ȭ
	if (ActivatedListenIconActor)
	{
		UWidgetComponent* WidgetComponent = ActivatedListenIconActor->GetWidgetComponent();

		if (!WidgetComponent)
		{
			return;
		}

		UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
		if (URtListenIconUserWidget* ListenIconWidget = Cast<URtListenIconUserWidget>(UserWidget))
		{
			ListenIconWidget->SetIsListening(false);
		}

		ActivatedListenIconActor = nullptr;
	}

	// Ȱ��ȭ�� ��������� �ִٸ�, ActivatedListenIconActor ������Ʈ
	const TArray<FString>& menuSetIDList = ScenarioModel->TargetDropboxMap.FindRef(CurNpcName);

	for (FString menuID : menuSetIDList)
	{
		if (ListenIconMap.Contains(menuID))
		{
			ARtWidgetActor_ListenIcon* CurListenIconActor = ListenIconMap.FindRef(menuID);
			if (!CurListenIconActor->GetIsVisible())
			{
				continue;
			}

			UWidgetComponent* WidgetComponent = CurListenIconActor->GetWidgetComponent();
			if (!WidgetComponent)
			{
				return;
			}

			UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
			if (URtListenIconUserWidget* ListenIconWidget = Cast<URtListenIconUserWidget>(UserWidget))
			{
				ListenIconWidget->SetIsListening(true);
			}

			ActivatedListenIconActor = CurListenIconActor;
		}
	}
}

void ARtGameScenarioController::ExecuteCommandToNpc(const FString& targetNpcName, const FString& commandName)
{
	if (targetNpcName.IsEmpty() || commandName.IsEmpty()) return;

	if (!ActivatedListenIconActor) return;

	FString menuID = ActivatedListenIconActor->GetTargetMenuName();
	TArray<FString> voiceEventIDs = ActivatedListenIconActor->GetVoiceCommandList();
	if (menuID.IsEmpty() || voiceEventIDs.Num() < 1) return;

	for (const FString& voiceEventID : voiceEventIDs)
	{
		if (commandName.Contains(voiceEventID))
		{
			UE_LOG(LogTemp, Warning, TEXT("Speech Recognizer Testing, commandName : %s, Found Command : %s"), *commandName, *menuID);

			if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld())) {
				
				TPair<const FRtEventCommand*, FString> data = gsc->ScenarioModel->ServerEventCommandMap.FindRef(menuID);
				const FRtEventCommand* commands = data.Key;

				if (commands == nullptr)
					return;

				Force_RequestRandomNum(&commands->ProcessCommandList);

				if (gsc->CurrentPlayType != ERt_PlayType::E_Single) {
					if (auto* nmgr = gsc->Get_NetManager()) {
						nmgr->SendPacket_SCTEventID(menuID);
					}
				}
				else {
					// �̱� �÷���
					gsc->ExecuteServerEvent(menuID);
				}

				for (const FRtSctFunctionCommand& command : commands->ProcessCommandList) {
					const ERtSctFunCmd& cmd = command.FunctionCommand;
					if (cmd == ERtSctFunCmd::RecordResultLog) {
						if (auto found = gsc->CallCommandFunctionMap.Find(cmd)) {
							bool retValue = false;
							if (found->ExecuteIfBound(command.Params, retValue))
							{
							}
						}
					}
				}

			}
		}
	}
}

//void ARtGameScenarioController::ExecuteMenuOption(ARtWidgetActor_MenuOption* MenuOption)
//{
//	MenuOption->Execute();
//}

void ARtGameScenarioController::AddAimMap(const int32& playerID, const FString& objectID)
{
	Resv_AimMap.Add(playerID, objectID);
}

void ARtGameScenarioController::AddShotList(const FString& weaponID, const FString& objectID, const FVector& hitPosition, const FVector& shotPosition, const FVector& shotDirection, const bool& amm)
{
	FString logStr = FString::Printf(TEXT("Resv Shot: %s"), *objectID);
	const uint64 screenKey = GetTypeHash(FName("Shot"));
	GEngine->AddOnScreenDebugMessage(screenKey, 5.f, FColor::Orange, logStr);
	Resv_ShotList.Add(FShotData(weaponID, objectID, hitPosition, shotPosition, shotDirection, amm));

	if (ARtObjectActor* weapon = Cast<ARtObjectActor>(GetSceneManagerRef()->FindObjectByName(weaponID))) {
		weapon->bGunFired = true;
	}
}

void ARtGameScenarioController::AddInteractKeyList(const int32& playerID)
{
	Resv_InteractKeyList.Add(playerID);
}

void ARtGameScenarioController::ChangeSelectionMenu(const float& value)
{
	return; // �޴��� �ڵ���Ʈ�ѷ� ����Ǿ� ���

	if (MenuWidget == nullptr)
		return;

	ARtWidgetActor* menuWidget = Cast<ARtWidgetActor>(MenuWidget.Get());
	if (menuWidget == nullptr)
		return;

	URt_Trainee_SelectionMenu* menu = Cast<URt_Trainee_SelectionMenu>(menuWidget->WidgetComponent->GetWidget());

	if (menu == nullptr)
		return;

	if (value < 0.0f)
		menu->SelectDown();
	else
		menu->SelectUp();
}

void ARtGameScenarioController::SelectMenu()
{
	if (MenuWidget != nullptr)
	{
		ARtTraineePawn* traineePawn = Cast<ARtTraineePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (traineePawn && !traineePawn->IsVR)
		{
			if (ARtWidgetActor_Menu* handMenu = Cast<ARtWidgetActor_Menu>(MenuWidget))
			{
				FVector shotStart = FVector::ZeroVector;
				shotStart = traineePawn->ShotStart->GetComponentLocation();
				FVector WorldDirection = shotStart + traineePawn->ShotStart->GetForwardVector() * 1000.0f;

				TArray<FHitResult> hitResults;
				TArray<AActor*> actorsToIgnore = { traineePawn };

				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

				if (UKismetSystemLibrary::LineTraceMultiForObjects(traineePawn->GetWorld(), shotStart, WorldDirection,
					ObjectTypes, true, actorsToIgnore, EDrawDebugTrace::None, hitResults, true, FLinearColor::Red, FLinearColor::Green, 1.0f))
				{
					for (const FHitResult& hitResult : hitResults)
					{
						if (ARtWidgetActor_MenuOption* option = Cast<ARtWidgetActor_MenuOption>(hitResult.GetActor()))
						{
							if (ARtWidgetActor_Menu* menuWidget = Cast<ARtWidgetActor_Menu>(MenuWidget.Get()))
							{
								menuWidget->OptionOverlapTime = 0.0f;
								menuWidget->HoverOption = option;

								return;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// �ٲ� ������
		if (ARtTraineePawn* character = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			character->WalkieTalkieTransmitStart();
		}
	}
}

void ARtGameScenarioController::UpdateNpcState(stNpcState& npcState)
{
	ENetMode netMode = GetNetMode();
	if (netMode != NM_Client)
		return;

	FString npcID = (TCHAR*)npcState.NpcID;

	if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(GetSceneManagerRef()->FindObjectByName(npcID)))
	{

		FRtNpcActState npcActState = npc->GetNpcActState();

		npc->StopMove();

		FRtNpcActState oldState = npc->GetNpcActState();
		ERtSctFunCmd sctFunCmd = (ERtSctFunCmd)npcState.SctFunCmd;
		ERtAnimType animType = (ERtAnimType)npcState.State;

		npcActState.SctFunCmd = sctFunCmd;
		npcActState.SctString.Empty();

#if WITH_EDITOR
		FILE_APP_LOG(
			"server_debug.log",
			"ARtGameScenarioController::UpdateNpcState() netmode : %d, npcID : %s, SctFunCmd : %s, AnimType : %s",
			netMode,
			*npcID,
			*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtSctFunCmd"), (int32)sctFunCmd),
			*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), (int32)animType)
		);
#endif

		if (DebugMode)
		{
			FString msg = FString::Printf(TEXT("Resv: %s, %d, %d"), *npcID, npcState.SctFunCmd, npcState.State);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, msg);
		}

		switch (animType)
		{
		case ERtAnimType::ECollapse:
		case ERtAnimType::ESurrender:
			npc->UpdateAnimState(animType);
			npcActState.Target.Reset();
			break;
		default:
			break;
		}

		switch (sctFunCmd)
		{
		case ERtSctFunCmd::ENone:
		{
			FString targetName = (TCHAR*)npcState.Data1.SctString;
			if (AActor* target = GetSceneManagerRef()->FindObjectByName(targetName))
			{
				npcActState.Target = MakeWeakObjectPtr(target);
			}
			else
			{
				for (TPair<FString, ARtTraineePawn*> traineePawnPair : ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap())
				{
					ARtTraineePawn* traineePawn = traineePawnPair.Value;

					// dave
					if (!IsValid(traineePawn) || !traineePawn->IsValidLowLevel())
						continue;

					if (auto* ps = traineePawn->Get_PlayerState())
					{
						auto& device_info = ps->Get_PlayerInfo();
						if (device_info.PlayerRoleID.Equals(targetName, ESearchCase::IgnoreCase))
						{
							npcActState.Target = MakeWeakObjectPtr(traineePawn);
							break;
						}
					}
				}
			}

			switch (animType)
			{
			case ERtAnimType::EWalk:
			{
				FVector freeMove = FVector(npcState.Data1.SctFloat, npcState.Data2.SctFloat, npcState.Data3.SctFloat);

				npcActState.Target.Reset();
				npc->MoveToLocation(freeMove);
			}
			break;
			case ERtAnimType::ERun:
				if (npcActState.Target.IsValid())
				{
					AActor* target = npcActState.Target.Get();
					if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target))
					{
						target = targetTrainee->GetTraineeLocActor();
					}
					npc->MoveToActor(target, (TCHAR*)npcState.Data2.SctString, animType);
				}
				break;
			default:
				break;
			}
			npc->UpdateAnimState(animType);
		}
		break;
		case ERtSctFunCmd::PlayAnim:
		{
			FString animName = (TCHAR*)npcState.Data2.SctString;

			if (!animName.IsEmpty())
			{
				if (oldState.SctFunCmd == ERtSctFunCmd::PlayAnim &&
					oldState.SctString == animName)
					return;

				npcActState.SctString = animName;

				npc->UpdateAnimState(ERtAnimType::ENone);
				npc->PlayAnimation(animName, npcState.Data3.SctBool);
			}
			else
			{
				npc->UpdateAnimState(animType);
			}
		}
		break;
		case ERtSctFunCmd::NpcMoveToWayPoint:
		case ERtSctFunCmd::AttackTarget:
		{
			FString targetName = (TCHAR*)npcState.Data1.SctString;
			if (AActor* target = GetSceneManagerRef()->FindObjectByName(targetName))
			{
				npcActState.Target = MakeWeakObjectPtr(target);
			}
			else
			{
				if (ARtTraineePawn* traineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(targetName))
				{
					npcActState.Target = MakeWeakObjectPtr(traineePawn);
				}
			}

			switch (animType)
			{
			case ERtAnimType::EWalk:
			case ERtAnimType::ERun:
				if (npcActState.Target.IsValid())
					npc->MoveToActor(npcActState.Target.Get(), (TCHAR*)npcState.Data2.SctString, animType);
				break;
			case ERtAnimType::EAttack:
				npc->UpdateAnimState(ERtAnimType::EAttack);
				break;
			default:
				FString animName = (TCHAR*)npcState.Data2.SctString;
				if (!animName.IsEmpty())
				{
					npc->PlayAnimation(animName, npcState.Data3.SctBool);
				}
				break;
			}
		}
		break;
		default:
			npcActState.Target.Reset();
			npc->UpdateAnimState(ERtAnimType::EIdle);
			break;
		}

		npc->UpdateNpcActState(npcActState);

	}
}

bool ARtGameScenarioController::ExecuteProcessCommands(const FRtEventCommand* pEvent, bool isReceive)
{
	if (!pEvent)
		return false;

	if (isReceive || ENetMode::NM_ListenServer == GetNetMode() || ERt_PlayType::E_Single == CurrentPlayType || ERt_PlayType::E_Server == CurrentPlayType)
	{
		bool isLoop = false;

		for (const FRtSctFunctionCommand& command : pEvent->ProcessCommandList)
		{
			const ERtSctFunCmd& cmd = command.FunctionCommand;
			switch (cmd)
			{
			case ERtSctFunCmd::RecordResultLog:
				break;

			case ERtSctFunCmd::LoopEvent:
				isLoop = true;
				break;

			default:
				auto found = CallCommandFunctionMap.Find(cmd);
				if (found)
				{
					bool retValue = false;
					if (found->ExecuteIfBound(command.Params, retValue))
					{
					}
				}
				break;
			}
		}

		return isLoop;
	}
	else
	{
		return false;
	}
}

void ARtGameScenarioController::Surrender(FString npcID)
{
	if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(GetSceneManagerRef()->FindObjectByName(npcID)))
	{
		FRtNpcBehaviorState state = npc->GetNpcBehaviorState();
		state.ActType = ERtAnimType::ESurrender;
		npc->UpdateNpcBehaviorState(state);
	}
}

bool ARtGameScenarioController::IsTargetMenuEnable(FString targetMenuName)
{
	return !DisabledTargetMenuList.Contains(targetMenuName);
}

void ARtGameScenarioController::MoveToWayPoint_Progress()
{
	if (MoveToWayPointMap.IsEmpty())
		return;

	TArray<ARtObjectActor*> endMoveActors;
	for (auto& moveMap : MoveToWayPointMap)
	{
		ARtObjectActor* moveActor = moveMap.Key;
		ARtObjectActor* wayPoint = moveMap.Value.Key;
		float speed = moveMap.Value.Value;

		FVector wpLoc = wayPoint->GetActorLocation();
		FVector prevLoc = moveActor->GetActorLocation();

		FVector prevActorToWP = (wayPoint->GetActorLocation() - moveActor->GetActorLocation()).GetSafeNormal();//����

		moveActor->SetActorLocation(moveActor->GetActorLocation() + prevActorToWP * speed * GetWorld()->GetDeltaSeconds());//�̵�

		FVector newActorToWP = (wayPoint->GetActorLocation() - moveActor->GetActorLocation()).GetSafeNormal();//�̵� �� ����

		FVector newLoc = moveActor->GetActorLocation();
		float dot = prevActorToWP.Dot(newActorToWP);

		if (prevActorToWP.Dot(newActorToWP) <= 0)//��ǥ�� �����ϰų� ��ǥ�� ����ġ��
		{
			moveActor->SetActorLocation(wayPoint->GetActorLocation());
			endMoveActors.Add(moveActor);
		}
	}
	for (ARtObjectActor* moveActor : endMoveActors)
		MoveToWayPointMap.Remove(moveActor);
}

bool ARtGameScenarioController::CreateTargetTextWidgetActor(ARtWidgetActor*& widgetActor, URt_Trainee_Text*& widget, FString text, AActor* target, FString& errorMsg)
{
	if (TargetTextWidgetMap.Contains({ text, target }))
	{
		widgetActor = TargetTextWidgetMap.FindRef({ text, target });
		widget = Cast<URt_Trainee_Text>(widgetActor->WidgetComponent->GetWidget());
		errorMsg = "Already Exist";
		return false;
	}

	widgetActor = ARtWidgetActor::CreateRtWidgetActor();//Create 3D Widget Actor

	if (widgetActor == nullptr)
	{
		errorMsg = "Make Widget Actor Fail";
		return false;
	}

	widget = Cast<URt_Trainee_Text>(widgetActor->Init(RtTraineeWidgetBlueprint::TraineeTextWidget));
	if (widget == nullptr)
	{
		widgetActor->DestroyWidgetActor();
		widgetActor = nullptr;
		errorMsg = "Make Widget Fail";
		return false;
	}

	widget->SetText(FText::FromString(*text));//Set Text
	widget->SetFontSize(120);//Set Text
	widgetActor->SetWidgetSize(widget->GetDrawSize().X, widget->GetDrawSize().Y);

	TargetTextWidgetMap.Add({ text, target }, widgetActor);

	return true;
}

bool ARtGameScenarioController::CreateMenuWidgetActor(ARtWidgetActor*& widgetActor, URt_Trainee_SelectionMenu*& widget, FString menuSetID, FString& errorMsg)
{
	//widgetActor = ARtWidgetActor::CreateRtWidgetActor();//Create 3D Widget Actor
	//if (widgetActor == nullptr)
	//{
	//	errorMsg = "Make Widget Actor Fail";
	//	return false;
	//}

	////Set Widget
	//widget = Cast<URt_Trainee_SelectionMenu>(widgetActor->Init(RtTraineeWidgetBlueprint::TraineeSelectionMenuWidget));
	//if (widget == nullptr)
	//{
	//	errorMsg = "Make Widget Fail";
	//	widgetActor->DestroyWidgetActor();
	//	return false;
	//}

	////Get Menu Commands
	//FRtDropboxCommand* dropboxCommand = ScenarioModel->DropboxCommandMap.Find(menuSetID);
	//if (dropboxCommand == nullptr)
	//{
	//	errorMsg = "Dropbox ID is Not Found";
	//	widgetActor->DestroyWidgetActor();
	//	return false;
	//}
	//if (dropboxCommand->DropboxElemantList.IsEmpty())
	//{
	//	errorMsg = "DropboxElemantList is Empty";
	//	widgetActor->DestroyWidgetActor();
	//	return false;
	//}

	//widget->SetMenuSetID(menuSetID);

	////Set Menu Commands
	//for (auto& data : dropboxCommand->DropboxElemantList)
	//{
	//	URt_Trainee_Selection* selection = widget->AddSelection(data.EventID, data.EventCommandName);
	//	if (selection == nullptr)
	//	{
	//		errorMsg = FString::Printf(TEXT("Add Selection Error : %s"), *data.EventID);
	//		widgetActor->DestroyWidgetActor();
	//		return false;
	//	}
	//}

	//if (MenuWidget.Get())
	//	if(ARtWidgetActor* menuWidget = Cast<ARtWidgetActor>(MenuWidget.Get()))
	//		menuWidget->DestroyWidgetActor();
	//MenuWidget = widgetActor;

	//widgetActor->SetWidgetSize(widget->GetDrawSize().X, widget->GetDrawSize().Y);

	return true;
}

void ARtGameScenarioController::RemoveTargetTextWidgetActor(FString text, AActor* target)
{
	if (target == nullptr)
		return;

	if (TargetTextWidgetMap.Contains({ text, target }) == false)
		return;

	ARtWidgetActor* widgetActor = TargetTextWidgetMap.FindRef({ text, target });
	if (widgetActor == nullptr)
	{
		TargetTextWidgetMap.Remove({ text, target });
		return;
	}

	widgetActor->DestroyWidgetActor();
	TargetTextWidgetMap.Remove({ text, target });
}

void ARtGameScenarioController::RemoveMenuWidgetActor(FString menuSetID)
{
	if (MenuWidget == nullptr)
		return;

	ARtWidgetActor* menuWidget = Cast<ARtWidgetActor>(MenuWidget.Get());
	if (menuWidget == nullptr)
		return;

	URt_Trainee_SelectionMenu* widget = Cast<URt_Trainee_SelectionMenu>(menuWidget->WidgetComponent->GetWidget());
	if ((widget && widget->GetMenuSetID() == menuSetID) ||
		menuWidget->WidgetComponent->GetWidget() == nullptr)
	{
		menuWidget->DestroyWidgetActor();
		MenuWidget = nullptr;
	}
}

URt_Trainee_Main* ARtGameScenarioController::GetMainWidget()
{
	if (!MainWidget.IsValid())
	{
		if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			MainWidget = MakeWeakObjectPtr<URt_Trainee_Main>(trainee->GetMainWidget());
		}
	}

	return MainWidget.Get();
}

ARtWidgetActor_Radio* ARtGameScenarioController::GetRadioWidget()
{
	if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		return trainee->RadioWidget;
	}

	return nullptr;
}

ARtWidgetActor_Menu* ARtGameScenarioController::GetTargetMenuWidget()
{
	if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		return trainee->TargetMenuWidget;
	}

	return nullptr;
}

bool ARtGameScenarioController::IsInRange(AActor* startActor, const FVector& startLoc, const FVector& startDir, AActor* target, const float& maxDistance, const float& maxAngle)
{
	if (!startActor)
		return false;

	FVector2D startLoc2D = FVector2D(startLoc);
	FVector2D startDir2D = FVector2D(startDir).GetSafeNormal();
	FVector2D targetLoc2D = FVector2D(target->GetActorLocation());
	FVector2D toTargetDir2D = (targetLoc2D - startLoc2D).GetSafeNormal();

	// ��� �Ÿ�
	float dist2D = FVector2D::Distance(startLoc2D, targetLoc2D);
	if (dist2D > maxDistance)
		return false;

	// ��� ���� ����
	float dotProduct = FVector2D::DotProduct(startDir2D, toTargetDir2D);
	float radians = FMath::Acos(dotProduct);
	float degrees = FMath::RadiansToDegrees(radians);
	if (degrees > maxAngle)
		return false;

	// ��ֹ� üũ
	FVector calcTargetLoc = target->GetActorLocation();

	if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(target))
	{
		if (const FRtTctStruct* tct = npc->GetTctStruct())
		{
			float maxHeight = 5.0f;
			for (const TPair<UShapeComponent*, ERtCollisionType>& collPair : npc->GetCollisionMap())
			{
				if (UCapsuleComponent* cap = Cast<UCapsuleComponent>(collPair.Key))
				{
					float newHeight = cap->GetScaledCapsuleHalfHeight();
					if (maxHeight < newHeight)
						maxHeight = newHeight;
				}
			}
			calcTargetLoc.Z += maxHeight * 2.0f - 10.0f;
		}
	}

	FVector endLoc = startLoc + (calcTargetLoc - startLoc).GetSafeNormal() * maxDistance * 2.0f;

	FHitResult hitResult;
	TArray<AActor*> actorsToIgnore = { startActor };
	if (AActor* parent = startActor->GetAttachParentActor())
	{
		actorsToIgnore.Add(parent);
	}

	EDrawDebugTrace::Type debugLineType = DebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), startLoc, endLoc,
		ObjectTypes, true, actorsToIgnore, debugLineType, hitResult, true, FLinearColor::Red, FLinearColor::Green, 3.0f))
	{
		AActor* hitActor = hitResult.GetActor();
		if (hitActor == target)
			return true;
	}

	return false;
}

// �� �÷��̾� ��Ʈ�ѷ� ã��
ARtPlayerController* ARtGameScenarioController::GetMyPlayerController()
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;
	
	// ��ü ��Ʈ�ѷ� ��ȸ
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->IsLocalController()) // �� �ڽ����� Ȯ��
		{
			return Cast<ARtPlayerController>(PC);
		}
	}

	return nullptr; // �� ã���� nullptr ��ȯ
}

// �� �Ʒ��� �� ã��
ARtTraineePawn* ARtGameScenarioController::GetMyTraineePawn()
{
	ARtPlayerController* myPlayerController = GetMyPlayerController();
	if(myPlayerController) {
		ERt_PlayType myPlayType = myPlayerController->Get_PlayType();

		if (myPlayType == ERt_PlayType::E_Trainee) {
			ARtPlayerState* myPlayerState = myPlayerController->Get_PlayerState();
			
			ARtTraineePawn* myTraineePawn = ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap().FindRef(myPlayerState->Get_PlayerInfo().PlayerRoleID);
			if (myTraineePawn) {
				return myTraineePawn;
			}
		}
	}

	return nullptr;
}

// �� �÷��̾� ������Ʈ�� ã��
ARtPlayerState* ARtGameScenarioController::GetMyPlayerState()
{
	ARtPlayerController* myPlayerController = GetMyPlayerController();
	if (myPlayerController) {
		return myPlayerController->Get_PlayerState();
	}
	return nullptr;
}

ARtGameState* ARtGameScenarioController::GetGameState()
{
	auto* myPlayerController = GetMyPlayerController();
	if (myPlayerController) {
		ARtGameState* gameState = myPlayerController->Get_GameState();
		return gameState;
	}

	return nullptr;
}
