// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtReplayManagerActor.h"

//Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtReplayManager.h"
#include "Manager/RtLoadingManager.h"
#include "RtGlobalActorManager.h"

//SCT
#include "Manager/RtGameSceneManager.h"
#include "Models/RtScenarioModel.h"
#include "Manager/RtGameScenarioController.h"
#include "RtResourceModel/Public/RtObjectActor.h"
#include "RtReadWriteFile.h"

//Motion
#include "Common/RtBlueprintAssetPath.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtReplayTrainee.h"
#include "Framework/RtNpcObjectActor.h"

//Web
#include "Common/RtWebCommon.h"
#include "Manager/RtWebManager.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerController.h"

//Json
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "glTFRuntimeFBXAssetActor.h"

// Sets default values
ARtReplayManagerActor::ARtReplayManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtReplayManagerActor::BeginPlay()
{
	Super::BeginPlay();

	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("ReplayManagerActor");
		gam->ManagerMap.Add({ key, this });
	}

	int recordSaveMode;
	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (!controller->Get_GameManager()->Is_Play_Type(ERt_PlayType::E_Single)) {
			if (FParse::Value(FCommandLine::Get(), TEXT("RecordSaveMode="), recordSaveMode))
			{
				recordSaveMode = FMath::Clamp(recordSaveMode, 0, 2);
				RecordSaveMode = (ESaveMode)recordSaveMode;
			}
		}
	}

	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController())) {
		if (controller->Get_GameManager()->Is_Play_Type(ERt_PlayType::E_Single)) {
			RecordSaveMode = ESaveMode::Save_None;
		}
	}

#if WITH_EDITOR

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.SetID"), TEXT("TID UID"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.Num() < 2)
				return;

			int32 tid = FCString::Atoi(*Args[0]);
			int32 uid = FCString::Atoi(*Args[1]);

			SetID(tid, uid);
			}));
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.SpawnTrainee"), TEXT("[UID] [Role]"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.Num() >= 2)
			{
				int32 uid = FCString::Atoi(*Args[0]);
				SpawnReplayTrainee(uid, *Args[1]);
			}
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.PrintTime"), TEXT("PrintTime"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			PrintTime();
			}));
	
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.SetTestTime"), TEXT("TestTime"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.IsEmpty())
				return;
			float newTime = FCString::Atof(*Args[0]);
			SetTestTime(newTime);
			Replaying(newTime);
			}));

#endif

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.SetMode"), TEXT("0:None 1:Server 2:LocalFile"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.IsEmpty())
				return;

			int mode = FCString::Atoi(*Args[0]);
			mode = FMath::Clamp(mode, 0, 2);
			RecordSaveMode = (ESaveMode)mode;
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.Load.Sct"), TEXT("[SctFileName] [TID]"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.Num() >= 1)
			{
				if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					ARtGameState* state = controller->Get_GameState();
					controller->Get_PlayerState()->Play_Type = ERt_PlayType::E_Replay;
					FRtScenarioInfo sceInfo = state->Get_Scenario_Info();
					sceInfo.SCTFileName = FString::Printf(TEXT("%s.sct.json"), *Args[0]);
					state->Set_Scenario_Info(sceInfo);

					controller->LoadSct();
					LoadRecordData(*Args[1]);
				}
			}
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.Play"), TEXT("[1/0] Start/End Replay"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			if (Args.IsEmpty())
				return;

			if (Args[0] != TEXT("0"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Start Replay"));
				PlayReplay();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("End Replay"));
				InitReplay();
			}
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Replay.Clear"), TEXT("Clear Replay Datas"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Clear Replay Datas"));
			ReplayMotionDataMap.Empty();
			RecordDataMap.Empty();
			CurrentDataMap.Empty();
			}));

	// 2025년 6월 버전을 위한 우회 처리 
	// 기존의 FBX를 읽어와 FBX 모델을 움직여야 정석이지만 제대로 되지 않는다. 
	// 이에 동일한 내장 모델로 교체하여 돌아가게만 처리한다.
	if (FbxFileToInternalAssetMap.IsEmpty())
	{
		FbxFileToInternalAssetMap.Add(TEXT("Character_Soldier_Sentry.FBX"), // 초병
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Soldier_Sentry/Mesh/Soldier_Sentry.Soldier_Sentry"));

		FbxFileToInternalAssetMap.Add(TEXT("Character_Soldier_Mobile.FBX"), // 기동소대
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Soldier_Mobile/Mesh/Soldier_Mobile.Soldier_Mobile"));

		FbxFileToInternalAssetMap.Add(TEXT("Character_Soldier_Special_Force.FBX"), // 특임대
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Soldier_Special_Forces/Mesh/Soldier_Special_Force.Soldier_Special_Force"));

		FbxFileToInternalAssetMap.Add(TEXT("Character_Soldier_EOD.FBX"), // EOD
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Soldier_EOD/Mesh/Soldier_EOD.Soldier_EOD"));

		FbxFileToInternalAssetMap.Add(TEXT("Character_Stranger2.FBX"), // 거수자 2
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Stranger_Two/Mesh/Stranger_Two.Stranger_Two"));

		FbxFileToInternalAssetMap.Add(TEXT("Character_Stranger3.FBX"), // 거수자 3
			TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Terrorist/Mesh/Terrorist.Terrorist"));
	}
}

void ARtReplayManagerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InitReplay();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ARtReplayManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsConsolePlay) {
		if (Replaying(TestTime)) {
//			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Stop Replay")));
			
			ReplayEndEvent.ExecuteIfBound();

			APP_LOG("Stop Replay !");
			IsConsolePlay = false;
			TestTime = 0.0f;
			CurrentReplayTimeRate = -1.0f;

			Replaying(TestTime);

			const TMap<FString, class ARtNpcObjectActor*>& NpcMap = ARtGameSceneManager::Get(GetWorld())->GetRtNpcMap();
			for (TPair<FString, ARtNpcObjectActor*> newTargetPair : NpcMap) {
				ARtNpcObjectActor* NpcActor = newTargetPair.Value;
				if (NpcActor) {
					NpcActor->StopAnimation();
				}
			}

		} else {
			TestTime += DeltaTime;

			CurrentReplayTimeRate = TestTime / TotalReplayTime;
		}
	}

}

void ARtReplayManagerActor::PostProcessEntities()
{
	TSubclassOf<UAnimInstance> npcAnimInstanceClass = LoadClass<UAnimInstance>(nullptr, RtBlueprintAsset::RtReplayTraineeAnim);

	for (TPair<FString, FReplayObjectDatas> pair : RecordDataMap)
	{
		ARtObjectActor* actor = ARtGameSceneManager::Get(GetWorld())->FindObjectByName(pair.Key);

		if (actor == nullptr)
			continue;

		switch (actor->GetTctType2())
		{
		case ERtTctType2::ECharacter:
			if (USkeletalMeshComponent* skel = actor->GetSkeletalMesh()) {
				if (npcAnimInstanceClass)
					skel->SetAnimClass(npcAnimInstanceClass);
			}
			break;
		default:
			break;
		}
	}

	Replaying(0.0f);

	// Loading Fin
	if (auto* gi = GetGameInstance()) {
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
			lm->OnLevelLoaded();
		}
	}

}

void ARtReplayManagerActor::InitReplay()
{
	GetWorld()->GetTimerManager().ClearTimer(RecordTimerHandle);
	RecordCount = 0;
	TestTime = 0.0f;
	IsConsolePlay = false;
	for (TPair<int32, ARtReplayTrainee*> pair : ReplayPawnMap)
	{
		if (ARtReplayTrainee* pawn = pair.Value)
			pawn->Destroy();
	}
	ReplayPawnMap.Empty();

	SaveEntityDatasForLocal.Empty();

	TotalReplayTime = -1.0f;
	CurrentReplayTimeRate = -1.0f;

	PlayerDistanceInfoMap.Empty();
}

void ARtReplayManagerActor::StartRecord()
{
	if (RecordSaveMode == ESaveMode::Save_None)
		return;

	TraineeUIDMap.Empty();
	// TODO: TraineeUIDMap.Add(trainee, uid);

	PlayerDistanceInfoMap.Empty();

	GetWorld()->GetTimerManager().SetTimer(RecordTimerHandle, FTimerDelegate::CreateUObject(
		this, &ARtReplayManagerActor::Record), ReplayInterval, true, 0.0f);
}

void ARtReplayManagerActor::PlayReplay()
{
	IsConsolePlay = true;

	InitGunFiredFlag();

	AllNpcAnimPlayOrStop(true);
}

void ARtReplayManagerActor::AllNpcAnimPlayOrStop(bool play)
{
	const TMap<FString, class ARtNpcObjectActor*>& NpcMap = ARtGameSceneManager::Get(GetWorld())->GetRtNpcMap();

	for (TPair<FString, ARtNpcObjectActor*> newTargetPair : NpcMap) {
		ARtNpcObjectActor* NpcActor = newTargetPair.Value;
		if (NpcActor) {
			USkeletalMeshComponent* skel = Cast<USkeletalMeshComponent>(NpcActor->GetCurrentMeshComponentForTransform());
			if (skel) {
				if (play) {
					skel->GlobalAnimRateScale = 1;
				} else {
					skel->GlobalAnimRateScale = 0;
				}
			}
		}
	}
}

void ARtReplayManagerActor::PauseReplay()
{
	IsConsolePlay = false;

	AllNpcAnimPlayOrStop(false);
}

void ARtReplayManagerActor::SpawnReplayTrainee(int32 uid, FString role)
{
	if (ARtReplayTrainee* traineePawn = GetWorld()->SpawnActor<ARtReplayTrainee>(LoadClass<ARtReplayTrainee>(nullptr, RtBlueprintAsset::RtReplayTrainee)))
	{
		ReplayPawnMap.Add(uid, traineePawn);
		traineePawn->RoleID = role;
		SetSkeletalMeshAsset(traineePawn, role);
		ReplayingMotion(0.0f);
	}
}

bool ARtReplayManagerActor::Replaying(float playTime)
{
	bool endMotion = ReplayingMotion(playTime);
	bool endEvent = ReplayingObjects(playTime);

	return endMotion && endEvent;
}

void ARtReplayManagerActor::WriteRecordDataFile()
{
	if (SaveEntityDatasForLocal.IsEmpty())
		return;

	TArray<TSharedPtr<FJsonValue>> localJsonDatas;
	for (TSharedPtr<FJsonObject> data : SaveEntityDatasForLocal)
		localJsonDatas.Add(MakeShareable(new FJsonValueObject(data)));

	TSharedRef<FJsonObject> localSaveJson = MakeShared<FJsonObject>();
	localSaveJson->SetArrayField("List", localJsonDatas);

	FString localSaveJsonString;
	TSharedRef<TJsonWriter<>> localSaveJsonWriterAr = TJsonWriterFactory<>::Create(&localSaveJsonString);
	if (!FJsonSerializer::Serialize(localSaveJson, localSaveJsonWriterAr))
		return;

	UGameInstance* gi = GetGameInstance();
	URtReplayManager* replayManager = gi->GetSubsystem<URtReplayManager>();
	FString dir = replayManager->MotionFilePath;

	if (dir.IsEmpty()) {

		dir = FString::Format(TEXT("{0}/TrainingData/RepFiles/{1}/{2}/Motion.json"),
			{ *FPaths::ProjectDir(),
			*(ARtGameSceneManager::Get(GetWorld())->CurrentSctFileName),
			GetTID() }
		);

		FPaths::MakeStandardFilename(dir);

		//FString fileName = FString::Printf(TEXT("%d/Motion.json"), GetTID());
		//dir = FPaths::Combine(*FPaths::ProjectDir(), TEXT("/TrainingData/RepFiles/INTERACT ETXR"), *fileName);
		//FPaths::MakeStandardFilename(dir);
	}

	FFileHelper::SaveStringToFile(*localSaveJsonString, *dir);
}

void ARtReplayManagerActor::SendAllPlayerDistanceLog()
{
	if (auto* gi = GetGameInstance()) {
		URtWebManager* webManager = gi->GetSubsystem<URtWebManager>();

		int32 tid = GetTID();

		// 모든 플레이어의 총 이동 거리를 로그로 보내준다.
		for (auto& item : PlayerDistanceInfoMap) {
			webManager->SendLogDistance(tid, item.Key, item.Value.TotalMovedDistance);
		}
	}
}

void ARtReplayManagerActor::Record()
{
	RecordCount++;
	PrintTime();

	TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();
	if (!jsonObject)
		return;

	SaveMotionDatas(jsonObject);
	SaveObjectsDatas(jsonObject);

	if (RecordSaveMode == ESaveMode::Server) {
		if (auto* gi = GetGameInstance()) {
			auto* WebSubsys = gi->GetSubsystem<URtWebManager>();
			if (WebSubsys->IsInitialized()) {
				FString saveJsonString;
				TSharedRef<TJsonWriter<>> saveJsonWriterAr = TJsonWriterFactory<>::Create(&saveJsonString);
				if (!FJsonSerializer::Serialize(jsonObject.ToSharedRef(), saveJsonWriterAr))
					return;

				WebSubsys->SendReplayMotion(GetTID(), saveJsonString);
			}
		}
	}
	else if (RecordSaveMode == ESaveMode::LocalFile) {
		
		SaveEntityDatasForLocal.Add(jsonObject);
	}
}

void ARtReplayManagerActor::LoadRecordData(FString folder)
{
	UGameInstance* gi = GetGameInstance();
	URtReplayManager* replayManager = gi->GetSubsystem<URtReplayManager>();
	FString dir = replayManager->MotionFilePath;
	
	//FString fileName = FString::Printf(TEXT("%s/Motion.json"), *folder);
	//FString dir = FPaths::Combine(*FPaths::ProjectDir(), TEXT("/TrainingData/RepFiles/INTERACT ETXR"), *fileName);
	//FPaths::MakeStandardFilename(dir);

	// Load
	FString loadString;
	if (!FFileHelper::LoadFileToString(loadString, *dir))
		return;

	SetEntitiesDatas(loadString);
}

void ARtReplayManagerActor::SaveMotionDatas(TSharedPtr<FJsonObject> jsonObject)
{
	TArray< TSharedPtr<FJsonValue> > motionDatas;

	for (TPair<FString, ARtTraineePawn*> traineePawnPair : ARtGameSceneManager::Get(GetWorld())->GetRtTraineeMap())
	{
		ARtTraineePawn* trainee = traineePawnPair.Value;		

		// 좀더 강력한 유효성 검사 추가
		if ( !(IsValid(trainee) && !trainee->IsPendingKill()) )
			continue;

		FMotionTransformData motionData;

		motionData.Scale = trainee->MotionScale;
		motionData.SetBoneDatas(trainee->SkeletalMesh);

		int32 uid = 0;

		if (ARtPlayerState* playerState = trainee->Get_PlayerState()) {
			uid = playerState->Get_PlayerInfo().TraineeUID;

			do {
				FPlayerDistanceInfo* found = PlayerDistanceInfoMap.Find(uid);
				if (found) {
					found->AccumulateDistance(motionData.Pelvis.GetLocation()); // 총 이동 거리 누적 계산.
					break;
				}
				else {
					FPlayerDistanceInfo& AddedItem = PlayerDistanceInfoMap.Add(uid, FPlayerDistanceInfo());
					AddedItem.Init(motionData.Pelvis.GetLocation());
				}
			} while (true);
		}

		TSharedPtr<FJsonObject> MotionObj = MakeShared<FJsonObject>();
		if (!MotionObj)
			return;

		SetIntField(MotionObj, "uid", uid);
		ARtReplayManagerActor::SetFloatField(MotionObj, "sclxy", motionData.Scale.X);
		ARtReplayManagerActor::SetFloatField(MotionObj, "sclz", motionData.Scale.Z);

		motionData.ConvertToMotionJson(MotionObj);

		motionDatas.Add(MakeShareable(new FJsonValueObject(MotionObj)));
	}

	jsonObject->SetArrayField("Mots", motionDatas);
}

bool ARtReplayManagerActor::ReplayingMotion(float playTime)
{
	bool isEnd = true;

	if (ReplayMotionDataMap.IsEmpty())
		return true;

	for (TPair<int32, FReplayMotionData> pair : ReplayMotionDataMap)
	{
		const FReplayMotionData& data = pair.Value;
		ARtReplayTrainee* replayPawn = ReplayPawnMap.FindRef(pair.Key);
		if (replayPawn == nullptr)
			continue;

		uint32 maxIndex = data.MotionDatas.Num() - 1;
		float curIndex = FMath::Clamp(playTime / ReplayInterval, 0.0f, maxIndex);

		uint32 prevIndex = FMath::FloorToInt32(curIndex);
		uint32 nextIndex = prevIndex;

		if (nextIndex < maxIndex)
		{
			nextIndex++;

			float alpha = playTime / ReplayInterval - prevIndex;
			replayPawn->MotionData = LerpMotionTransformData(data.MotionDatas[prevIndex], data.MotionDatas[nextIndex], alpha);
			replayPawn->UpdateRenderTargetCamera();

			//FTransform AddTransform;
			//AddTransform.SetLocation(FVector(500.1f, 0, 0));
			//replayPawn->MotionData.HandLeft.Accumulate(AddTransform);

			isEnd = false;
		}
		else
		{
			replayPawn->MotionData = data.MotionDatas[maxIndex];
			replayPawn->UpdateRenderTargetCamera();
		}
	}

	//if (isEnd && IsLoop)
	//{
	//	CurTime = 0.0f;
	//
	//	for (TPair<int32, FReplayMotionData> pair : ReplayMotionDataMap)
	//	{
	//		const FReplayMotionData& data = pair.Value;
	//		if (ARtReplayTrainee* replayPawn = ReplayPawnMap.FindRef(pair.Key))
	//			replayPawn->MotionData = data.MotionDatas[0];
	//	}
	// 
	//  return false;
	//}

	return isEnd;
}

FMotionTransformData ARtReplayManagerActor::LerpMotionTransformData(const FMotionTransformData& prevData, const FMotionTransformData& nextData, float alpha)
{
	FMotionTransformData resultMotionData;

	resultMotionData.Scale = prevData.Scale;
	resultMotionData.Head = LerpTransform(prevData.Head, nextData.Head, alpha);
	resultMotionData.Pelvis = LerpTransform(prevData.Pelvis, nextData.Pelvis, alpha);
	resultMotionData.HandLeft = LerpTransform(prevData.HandLeft, nextData.HandLeft, alpha);
	resultMotionData.HandRight = LerpTransform(prevData.HandRight, nextData.HandRight, alpha);
	resultMotionData.FootLeft = LerpTransform(prevData.FootLeft, nextData.FootLeft, alpha);
	resultMotionData.FootRight = LerpTransform(prevData.FootRight, nextData.FootRight, alpha);
	resultMotionData.UpperArmLeft = LerpTransform(prevData.UpperArmLeft, nextData.UpperArmLeft, alpha);
	resultMotionData.UpperArmRight = LerpTransform(prevData.UpperArmRight, nextData.UpperArmRight, alpha);

	return resultMotionData;
}

void ARtReplayManagerActor::SaveObjectsDatas(TSharedPtr<FJsonObject> jsonObject)
{
	TArray< TSharedPtr<FJsonValue> > objectDatas;
	for (TWeakObjectPtr<ARtObjectActor> objectPtr : RecordObjects)
	{
		ARtObjectActor* object = objectPtr.Get();

		FString objectID = object->GetObjectName();
		if (objectID.IsEmpty())
			continue;

		TSharedRef<FJsonObject> objectDataJson = MakeShared<FJsonObject>();

		FRecordObjectData data;
		data.ObjectID = objectID;

		data.ConvertDataToJson(objectDataJson, object);

		objectDatas.Add(MakeShareable(new FJsonValueObject(objectDataJson)));
	}
	jsonObject->SetArrayField("Objs", objectDatas);

}

void ARtReplayManagerActor::SetEntitiesDatas(const FString& replayDatas)
{
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> datasReader = TJsonReaderFactory<>::Create(replayDatas);
	if (!FJsonSerializer::Deserialize(datasReader, jsonObject))
		return;
	TArray<TSharedPtr<FJsonValue>> datalist = jsonObject->GetArrayField("List");

	RecordDataMap.Empty();
	CurrentDataMap.Empty();
	DataSize = datalist.Num();

	if (TotalReplayTime < 0) {
		TotalReplayTime = (float)DataSize / 10.0f;
	}

	ReplayMotionDataMap.Empty();
	RecordDataMap.Empty();

	for (TSharedPtr<FJsonValue> JsonValue : datalist)
	{
		TArray<TSharedPtr<FJsonValue>> motionlist = JsonValue->AsObject()->GetArrayField("Mots");
		for (TSharedPtr<FJsonValue> motionJsonValue : motionlist)
		{
			TSharedPtr<FJsonObject> MotionObj = motionJsonValue->AsObject();

			int32 uid = MotionObj->GetNumberField(TEXT("uid"));

			FReplayMotionData replayMotionData;
			if (ReplayMotionDataMap.Contains(uid)) {
				replayMotionData = ReplayMotionDataMap.FindRef(uid);
			}

			FMotionTransformData mData;
			mData.Scale.X = MotionObj->GetNumberField(TEXT("sclxy"));
			mData.Scale.Y = mData.Scale.X;
			mData.Scale.Z = MotionObj->GetNumberField(TEXT("sclz"));

			mData.ConvertFromMotionJson(MotionObj);

			replayMotionData.MotionDatas.Add(mData);
			ReplayMotionDataMap.Add(uid, replayMotionData);
		}

		TArray<TSharedPtr<FJsonValue>> objectDataList = JsonValue->AsObject()->GetArrayField("Objs");
		for (TSharedPtr<FJsonValue> dataJson : objectDataList)
		{
			TSharedPtr<FJsonObject> objectDataJson = dataJson->AsObject();

			FString objectID = objectDataJson->GetStringField(TEXT("id"));
			if (objectID.IsEmpty())
				continue;

			FReplayObjectDatas replayData;
			if (RecordDataMap.Contains(objectID)) {
				replayData = RecordDataMap.FindRef(objectID);
			}

			bool vis = objectDataJson->GetNumberField(TEXT("vis")) != 0;
			FVector scale = FVector(
				objectDataJson->GetNumberField(TEXT("sclX")),
				objectDataJson->GetNumberField(TEXT("sclY")),
				objectDataJson->GetNumberField(TEXT("sclZ")));
			FVector location = FVector(
				objectDataJson->GetNumberField(TEXT("locX")),
				objectDataJson->GetNumberField(TEXT("locY")),
				objectDataJson->GetNumberField(TEXT("locZ")));
			FRotator rotator = FRotator(
				objectDataJson->GetNumberField(TEXT("rotP")),
				objectDataJson->GetNumberField(TEXT("rotY")),
				objectDataJson->GetNumberField(TEXT("rotR")));

			FRecordObjectData recordData;
			recordData.ObjectID = objectID;
			recordData.Visible = vis;
			recordData.ActorTransform.SetScale3D(scale);
			recordData.ActorTransform.SetLocation(location);
			recordData.ActorTransform.SetRotation(rotator.Quaternion());
			recordData.CharacterMotionData.ConvertFromMotionJson(objectDataJson);
			recordData.CharacterMotionData.Scale = scale;
					
			recordData.NpcAnimFileName = objectDataJson->GetStringField(TEXT("ani"));

			int32 fired = (int32)objectDataJson->GetNumberField(TEXT("fired"));
			recordData.bGunFired = (fired == 1);

			replayData.RecordDatas.Add(recordData);

			RecordDataMap.Add(objectID, replayData);

		}
	}
}

void ARtReplayManagerActor::InitGunFiredFlag()
{
	for (auto& item : RecordDataMap) {
		for (auto& item2 : item.Value.RecordDatas) {
			item2.bGunFiredPlayOk = false;
		}
	}
}

bool ARtReplayManagerActor::ReplayingObjects(float playTime)
{
	bool isEnd = true;

	if (RecordDataMap.IsEmpty())
		return true;

	uint32 maxIndex = DataSize - 1;
	float curIndex = FMath::Clamp(playTime / ReplayInterval, 0.0f, maxIndex);

	uint32 prevIndex = FMath::FloorToInt32(curIndex);
	uint32 nextIndex = prevIndex;
	float alpha = 0;
	if (nextIndex < maxIndex) {
		nextIndex++;
		alpha = playTime / ReplayInterval - prevIndex;
		isEnd = false;
	}

	for (TWeakObjectPtr<ARtObjectActor> object : RecordObjects)
	{
		FString objectId = object.Get()->GetObjectName();

		if (!RecordDataMap.Contains(objectId))
			continue;

		const FReplayObjectDatas& data = RecordDataMap.FindRef(objectId);

		FRecordObjectData curData = data.RecordDatas[prevIndex];

		bool bMeshTransform = true;
		if (nextIndex < maxIndex)
		{
			switch (object.Get()->GetTctType2())
			{
			case ERtTctType2::ECharacter:
//				curData.CharacterMotionData = LerpMotionTransformData(data.RecordDatas[prevIndex].CharacterMotionData, data.RecordDatas[nextIndex].CharacterMotionData, alpha);
				if (!data.RecordDatas[prevIndex].NpcAnimFileName.IsEmpty()) {

					TCHAR systemInfo = data.RecordDatas[prevIndex].NpcAnimFileName[0];
					TCHAR loopInfo = data.RecordDatas[prevIndex].NpcAnimFileName[1];
					FString fileName = data.RecordDatas[prevIndex].NpcAnimFileName.Mid(3);
					
					bool bIsSystem = false;
					bool bLoop = false;
					FString finalFileName;
					if (systemInfo == TEXT('S')) { // system animation file
						finalFileName = TEXT("System/SysDefaultAnimFiles/") + fileName;
						bIsSystem = true;
					} else {
						finalFileName = fileName;
					}

					bLoop = (loopInfo == TEXT('L'));
					object.Get()->PlayAnimation(finalFileName, bLoop, -1, bIsSystem);
				}

				curData.ActorTransform = LerpTransform(data.RecordDatas[prevIndex].ActorTransform, data.RecordDatas[nextIndex].ActorTransform, alpha);
				break;
			case ERtTctType2::EEquipment: // 장착 장비.

				curData.ActorTransform = LerpTransform(data.RecordDatas[prevIndex].ActorTransform, data.RecordDatas[nextIndex].ActorTransform, alpha);

				if (data.RecordDatas[prevIndex].bGunFired) {
					// 이곳에서 Shot 처리

					if (!data.RecordDatas[prevIndex].bGunFiredPlayOk) {

						// 사운드 출력
						object.Get()->PlaySound();

						FVector pos = curData.ActorTransform.GetLocation();
						FRotator rot = FRotator(curData.ActorTransform.GetRotation());

						FTransform locTransform;
						if (object->GetFirstDirectionEffectTransform(locTransform))
						{
							FVector locPos = locTransform.GetLocation();
							locPos.Y += 50.0f;
							locPos.Z -= 40.0f;
							pos += curData.ActorTransform.TransformVector(locPos);
						}

						// 총구 효과 출력 ( 총구 효과는 데이터 기반으로 작업은 안하고 다음 나이아가라 이펙트로 고정해서 사용한다 )
						object->PlayNiagaraEffect(K2C1_FIRE_EFFECT1_PATH, pos, rot, 1);

						//FVector offSet = FVector(0, 0, 0);// curData.ActorTransform.GetTranslation();
						//object->PlaySingleEffect(ERtComponentType::EEffect, offSet);

						RecordDataMap[objectId].RecordDatas[prevIndex].bGunFiredPlayOk = true;
					}
				}

				object.Get()->SetActorTransform(curData.ActorTransform);

				bMeshTransform = false;
				break;
			default:
				curData.ActorTransform = LerpTransform(data.RecordDatas[prevIndex].ActorTransform, data.RecordDatas[nextIndex].ActorTransform, alpha);
				break;
			}
		}
		else {
			curData = data.RecordDatas[maxIndex];
		}

		object.Get()->SetVisible(curData.Visible);
		if (bMeshTransform) {
			object.Get()->GetCurrentMesh()->SetWorldTransform(curData.ActorTransform);
		}

		//object.Get()->SetActorTransform(curData.ActorTransform);
		CurrentDataMap.Add(object.Get(),curData);
	}

	//if (NpcReplayMotionDataMap.IsEmpty())
	//	return true;

	//for (TPair<FString, FReplayMotionData> pair : NpcReplayMotionDataMap)
	//{
	//	const FReplayMotionData& data = pair.Value;
	//	ARtNpcObjectActor* replayActor = NpcReplayPawnMap.FindRef(pair.Key);
	//	if (replayActor == nullptr)
	//		continue;

	//	uint32 maxIndex = data.MotionDatas.Num() - 1;
	//	float curIndex = FMath::Clamp(playTime / ReplayInterval, 0.0f, maxIndex);

	//	uint32 prevIndex = FMath::FloorToInt32(curIndex);
	//	uint32 nextIndex = prevIndex;

	//	if (nextIndex < maxIndex)
	//	{
	//		nextIndex++;

	//		float alpha = playTime / ReplayInterval - prevIndex;
	//		replayActor->MotionData = LerpMotionTransformData(data.MotionDatas[prevIndex], data.MotionDatas[nextIndex], alpha);

	//		isEnd = false;
	//	}
	//	else
	//	{
	//		replayActor->MotionData = data.MotionDatas[maxIndex];
	//	}
	//}


	return isEnd;
}

void ARtReplayManagerActor::SetCurrentPlayTime(float currentPlayTimeRate)
{
	TestTime = TotalReplayTime * currentPlayTimeRate;

	if (!IsConsolePlay)
	{
		Replaying(TestTime);
		AllNpcAnimPlayOrStop(false);
	}

	CurrentReplayTimeRate = TestTime / TotalReplayTime;
}

FTransform ARtReplayManagerActor::LerpTransform(FTransform a, FTransform b, float alpha)
{
	FTransform result;
	result.SetLocation(FMath::Lerp(a.GetLocation(), b.GetLocation(), alpha));
	result.SetRotation(FMath::Lerp(a.GetRotation(), b.GetRotation(), alpha));
	result.SetScale3D(FMath::Lerp(a.GetScale3D(), b.GetScale3D(), alpha));

	return result;
}

int32 ARtReplayManagerActor::GetTID()
{
	if (TID == nullptr)
	{
		if (ARtGameState* gs = GetWorld()->GetGameState<ARtGameState>())
			TID = &(gs->Training_Index);
	}

	return *TID;
}

void ARtReplayManagerActor::LoadSctReplay(const FRtReplayStruct& replayInfo)
{
	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		ARtGameState* state = controller->Get_GameState();
		FRtScenarioInfo sceInfo = state->Get_Scenario_Info();
		sceInfo.SCTFileName = FString::Printf(TEXT("%s.sct.json"), *replayInfo.SelectedScenarioName);
		state->Set_Scenario_Info(sceInfo);

		controller->LoadSct();

		UGameInstance* gi = GetGameInstance();
		URtReplayManager* replayManager = gi->GetSubsystem<URtReplayManager>();
		replayManager->MotionFilePath = replayInfo.SelectedScenarioFolderPath + TEXT("/") + TEXT("Motion.json");
		LoadRecordData(TEXT("")); // FString::FromInt(replayInfo.SelectedTID));
		for (int i = 0; i < replayInfo.UserIDs.Num(); i++) {
		    SpawnReplayTrainee(replayInfo.UserIDs[i], replayInfo.TraineeRoles[i]);
		}
	}
}

void ARtReplayManagerActor::SetIntField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const int value)
{
	jsonObj->SetStringField(key, FString::Printf(TEXT("%d"), value));
}

void ARtReplayManagerActor::SetIntField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const double value)
{
	jsonObj->SetStringField(key, FString::Printf(TEXT("%.f"), value));
}

void ARtReplayManagerActor::ARtReplayManagerActor::SetFloatField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const float value)
{
	jsonObj->SetStringField(key, FString::Printf(TEXT("%.2f"), value));
}

//void ARtReplayManagerActor::SetStringField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const FString value)
//{
//	jsonObj->SetStringField(key, FString::Printf(TEXT("%.2f"), value));
//}

ARtReplayManagerActor* ARtReplayManagerActor::Get(UObject* worldContext)
{
	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(worldContext)->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("ReplayManagerActor");

		if (AActor* findActor = gam->ManagerMap.FindRef(key)) {
			return Cast<ARtReplayManagerActor>(findActor);
		}
		if (AActor* findActor = UGameplayStatics::GetActorOfClass(worldContext, ARtReplayManagerActor::StaticClass())) {
			return Cast<ARtReplayManagerActor>(findActor);
		}
	}

	return nullptr;
}

void ARtReplayManagerActor::SetID(int32 tid, int32 uid)
{
	if (ARtGameState* gs = GetWorld()->GetGameState<ARtGameState>())
		gs->Training_Index = tid;

	if (ARtPlayerController* rtPlayerController = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		FRtDeviceInfo currinfo;
		if (rtPlayerController->Get_DeviceInfo(currinfo))
		{
			currinfo.PlayerInfo.UID = uid;
		}

		if (auto* ps = rtPlayerController->Get_PlayerState())
		{
			ps->Set_PlayerInfo(currinfo);
		}
	}
}

void ARtReplayManagerActor::PrintTime()
{
	const uint64 ScreenKey = GetTypeHash(FName("RecordCount"));
	GEngine->AddOnScreenDebugMessage(ScreenKey, 3.f, FColor::Orange, FString::Printf(TEXT("RecordCount: %d"), RecordCount));
}

void ARtReplayManagerActor::SetTestTime(float newTime)
{
	TestTime = newTime;
}

void ARtReplayManagerActor::SetSkeletalMeshAsset(ARtReplayTrainee* pawn, const FString& role)
{
	USkeletalMesh* skeletalMesh = nullptr;
	FString modelPath = ARtGameScenarioController::Get(GetWorld())->GetModelPathByObjectID(role);

	FString* foundValue = FbxFileToInternalAssetMap.Find(modelPath);
	if (foundValue) {
		modelPath = *foundValue;
	}
	
	//modelPath = TEXT("/Game/CommonModels/SkeletalModels/Models/Character/Soldier_Sentry/Mesh/Soldier_Sentry.Soldier_Sentry");

	if (!modelPath.IsEmpty())
	{
		// Load Lagacy SkeletalMesh Asset
		skeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
	}

	if (nullptr != skeletalMesh)
	{
		// Load Lagacy SkeletalMesh Asset
		//pawn->IsPoseableMeshPose = false;
		pawn->SkeletalMesh.Get()->SetSkeletalMesh(skeletalMesh);
	}
	else
	{
		//return;
		// Load GlTF SkeletalMesh Asset;
		
//		if (modelPath.Contains(TEXT(".fbx"))) {
//
//			FString fbxPath = FString::Format(TEXT("{0}/TrainingData/FbxFiles/{1}"), {*FPaths::ProjectDir(), *modelPath});
//
//			// 스켈레톤 메시 FBX
//			UglTFRuntimeAsset* gltfAsset = URtResourceModelSubsystem::Get()->LoadGltfAssetSub(fbxPath);
//			UWorld* pWorld = GetWorld();
//			AglTFRuntimeFBXAssetActor* glTFRuntimeFbxAssetActor = pWorld->SpawnActor<AglTFRuntimeFBXAssetActor>(
//				AglTFRuntimeFBXAssetActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
//
//				if (gltfAsset && glTFRuntimeFbxAssetActor) {
//					glTFRuntimeFbxAssetActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
//					glTFRuntimeFbxAssetActor->Asset = gltfAsset;
//					glTFRuntimeFbxAssetActor->ForceFBXLoadAsset();
//
//					TArray<UActorComponent*> SkeletalMeshComArr = glTFRuntimeFbxAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());
//					for (auto com : SkeletalMeshComArr) {
//						USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
//						if (skelMeshCom)
//						{
//							pawn->SkeletalMesh.Get()->SetSkeletalMesh(skelMeshCom->GetSkeletalMeshAsset());
////							skelMeshCom->AnimClass = pawn->SkeletalMesh.Get()->AnimClass;
////							pawn->SkeletalMesh = skelMeshCom;
//// 
//// 
//////							pawn->SkeletalMesh.Get()->setanim
////							pawn->SkeletalMesh.Get()->Activate(true);
//							break;
//						}
//					}
//				}
//		}
	}

	//pawn->SetMeshVisible(pawn->SkeletalMesh->IsVisible() || pawn->PoseableMesh->IsVisible());
}

void FMotionTransformData::SetBoneDatas(USkeletalMeshComponent* skeletalMesh)
{
	if (!IsValid(skeletalMesh))
		return;

	Head = skeletalMesh->GetSocketTransform(FName(TEXT("head")));
	Pelvis = skeletalMesh->GetSocketTransform(FName(TEXT("pelvis")));
	HandLeft = skeletalMesh->GetSocketTransform(FName(TEXT("hand_l")));
	HandRight = skeletalMesh->GetSocketTransform(FName(TEXT("hand_r")));
	FootLeft = skeletalMesh->GetSocketTransform(FName(TEXT("foot_l")));
	FootRight = skeletalMesh->GetSocketTransform(FName(TEXT("foot_r")));
	UpperArmLeft = skeletalMesh->GetSocketTransform(FName(TEXT("upperarm_l")));
	UpperArmRight = skeletalMesh->GetSocketTransform(FName(TEXT("upperarm_r")));

}

void FMotionTransformData::ConvertToMotionJson(TSharedPtr<FJsonObject> motionData)
{
	ARtReplayManagerActor::SetFloatField(motionData, "hlocx", Head.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "hlocy", Head.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "hlocz", Head.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "lhlocx", HandLeft.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "lhlocy", HandLeft.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "lhlocz", HandLeft.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "rhlocx", HandRight.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "rhlocy", HandRight.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "rhlocz", HandRight.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "pvlocx", Pelvis.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "pvlocy", Pelvis.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "pvlocz", Pelvis.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "lflocx", FootLeft.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "lflocy", FootLeft.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "lflocz", FootLeft.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "rflocx", FootRight.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "rflocy", FootRight.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "rflocz", FootRight.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "luplocx", UpperArmLeft.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "luplocy", UpperArmLeft.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "luplocz", UpperArmLeft.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "ruplocx", UpperArmRight.GetLocation().X);
	ARtReplayManagerActor::SetFloatField(motionData, "ruplocy", UpperArmRight.GetLocation().Y);
	ARtReplayManagerActor::SetFloatField(motionData, "ruplocz", UpperArmRight.GetLocation().Z);

	ARtReplayManagerActor::SetFloatField(motionData, "hrotr", Head.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "hrotp", Head.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "hroty", Head.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "lhrotr", HandLeft.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "lhrotp", HandLeft.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "lhroty", HandLeft.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "rhrotr", HandRight.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "rhrotp", HandRight.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "rhroty", HandRight.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "pvrotr", Pelvis.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "pvrotp", Pelvis.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "pvroty", Pelvis.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "lfrotr", FootLeft.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "lfrotp", FootLeft.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "lfroty", FootLeft.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "rfrotr", FootRight.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "rfrotp", FootRight.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "rfroty", FootRight.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "luprotr", UpperArmLeft.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "luprotp", UpperArmLeft.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "luproty", UpperArmLeft.GetRotation().Rotator().Yaw);

	ARtReplayManagerActor::SetFloatField(motionData, "ruprotr", UpperArmRight.GetRotation().Rotator().Roll);
	ARtReplayManagerActor::SetFloatField(motionData, "ruprotp", UpperArmRight.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(motionData, "ruproty", UpperArmRight.GetRotation().Rotator().Yaw);
}

void FMotionTransformData::ConvertFromMotionJson(TSharedPtr<FJsonObject> motionData)
{
	FVector loc = FVector::ZeroVector;
	FRotator rot = FRotator::ZeroRotator;

	loc.X = motionData->GetNumberField(TEXT("hlocx"));
	loc.Y = motionData->GetNumberField(TEXT("hlocy"));
	loc.Z = motionData->GetNumberField(TEXT("hlocz"));
	Head.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("lhlocx"));
	loc.Y = motionData->GetNumberField(TEXT("lhlocy"));
	loc.Z = motionData->GetNumberField(TEXT("lhlocz"));
	HandLeft.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("rhlocx"));
	loc.Y = motionData->GetNumberField(TEXT("rhlocy"));
	loc.Z = motionData->GetNumberField(TEXT("rhlocz"));
	HandRight.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("lflocx"));
	loc.Y = motionData->GetNumberField(TEXT("lflocy"));
	loc.Z = motionData->GetNumberField(TEXT("lflocz"));
	FootLeft.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("rflocx"));
	loc.Y = motionData->GetNumberField(TEXT("rflocy"));
	loc.Z = motionData->GetNumberField(TEXT("rflocz"));
	FootRight.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("pvlocx"));
	loc.Y = motionData->GetNumberField(TEXT("pvlocy"));
	loc.Z = motionData->GetNumberField(TEXT("pvlocz"));
	Pelvis.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("luplocx"));
	loc.Y = motionData->GetNumberField(TEXT("luplocy"));
	loc.Z = motionData->GetNumberField(TEXT("luplocz"));
	UpperArmLeft.SetLocation(loc);

	loc.X = motionData->GetNumberField(TEXT("ruplocx"));
	loc.Y = motionData->GetNumberField(TEXT("ruplocy"));
	loc.Z = motionData->GetNumberField(TEXT("ruplocz"));
	UpperArmRight.SetLocation(loc);

	rot.Roll = motionData->GetNumberField(TEXT("hrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("hrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("hroty"));
	Head.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("lhrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("lhrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("lhroty"));
	HandLeft.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("rhrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("rhrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("rhroty"));
	HandRight.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("lfrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("lfrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("lfroty"));
	FootLeft.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("rfrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("rfrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("rfroty"));
	FootRight.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("pvrotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("pvrotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("pvroty"));
	Pelvis.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("luprotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("luprotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("luproty"));
	UpperArmLeft.SetRotation(rot.Quaternion());

	rot.Roll = motionData->GetNumberField(TEXT("ruprotr"));
	rot.Pitch = motionData->GetNumberField(TEXT("ruprotp"));
	rot.Yaw = motionData->GetNumberField(TEXT("ruproty"));
	UpperArmRight.SetRotation(rot.Quaternion());

}

FString FMotionTransformData::ToString()
{
	FString result;
	result = FString::Printf(TEXT("CurFrame:\nScale(%s)\nHead(%s)\nPelvis(%s)\nHandLeft(%s)\nHandRight(%s)\nFootLeft(%s)\nFootRight(%s)\nUpperArmLeft(%s)\nUpperArmRight(%s)\n"), *Scale.ToString(), *ToString(Head), *ToString(Pelvis), *ToString(HandLeft), *ToString(HandRight), *ToString(FootLeft), *ToString(FootRight), *ToString(UpperArmLeft), *ToString(UpperArmRight));

	return result;
}

FString FMotionTransformData::ToString(const FTransform& transform)
{
	FString result;

	result = TEXT("Loc: ") + transform.GetLocation().ToString() + TEXT(" Rot: ") + transform.GetRotation().Rotator().ToString();

	return result;
}

void FRecordObjectData::ConvertDataToJson(TSharedRef<FJsonObject> jsonObject, ARtObjectActor* actor)
{
	Visible = actor->IsVisible();
	ActorTransform.SetScale3D(actor->GetCurrentMesh()->GetComponentScale());
	ActorTransform.SetLocation(actor->GetCurrentMesh()->GetComponentLocation());
	ActorTransform.SetRotation(actor->GetCurrentMesh()->GetComponentRotation().Quaternion());

	jsonObject->SetStringField("id", ObjectID);
	ARtReplayManagerActor::SetIntField(jsonObject, "vis", Visible);
	ARtReplayManagerActor::SetFloatField(jsonObject, "sclX", ActorTransform.GetScale3D().X);
	ARtReplayManagerActor::SetFloatField(jsonObject, "sclY", ActorTransform.GetScale3D().Y);
	ARtReplayManagerActor::SetFloatField(jsonObject, "sclZ", ActorTransform.GetScale3D().Z);

	ARtReplayManagerActor::SetIntField(jsonObject, "locX", ActorTransform.GetLocation().X);
	ARtReplayManagerActor::SetIntField(jsonObject, "locY", ActorTransform.GetLocation().Y);
	ARtReplayManagerActor::SetIntField(jsonObject, "locZ", ActorTransform.GetLocation().Z);
	ARtReplayManagerActor::SetFloatField(jsonObject, "rotP", ActorTransform.GetRotation().Rotator().Pitch);
	ARtReplayManagerActor::SetFloatField(jsonObject, "rotY", ActorTransform.GetRotation().Rotator().Yaw);
	ARtReplayManagerActor::SetFloatField(jsonObject, "rotR", ActorTransform.GetRotation().Rotator().Roll);

	if (actor->GetTctType2() == ERtTctType2::ECharacter) {
		jsonObject->SetStringField(TEXT("ani"), actor->CurrentAnimationFBXFileName);
	}
	
	if (actor->bGunFired) {
		jsonObject->SetStringField(TEXT("fired"), TEXT("1"));
		actor->bGunFired = false;
	}

	//switch (actor->GetTctType2())
	//{
	//case ERtTctType2::ECharacter:
	//	CharacterMotionData.SetBoneDatas(actor->GetSkeletalMesh());
	//	CharacterMotionData.ConvertToMotionJson(jsonObject);
	//	break;
	//default:
	//	ARtReplayManagerActor::SetIntField(jsonObject, "locX", ActorTransform.GetLocation().X);
	//	ARtReplayManagerActor::SetIntField(jsonObject, "locY", ActorTransform.GetLocation().Y);
	//	ARtReplayManagerActor::SetIntField(jsonObject, "locZ", ActorTransform.GetLocation().Z);
	//	ARtReplayManagerActor::SetFloatField(jsonObject, "rotP", ActorTransform.GetRotation().Rotator().Pitch);
	//	ARtReplayManagerActor::SetFloatField(jsonObject, "rotY", ActorTransform.GetRotation().Rotator().Yaw);
	//	ARtReplayManagerActor::SetFloatField(jsonObject, "rotR", ActorTransform.GetRotation().Rotator().Roll);
	//	break;
	//}

}

void FPlayerDistanceInfo::Init(FVector currentLocation)
{
	CurrentLocation = currentLocation;
}

void FPlayerDistanceInfo::AccumulateDistance(FVector currentLocation)
{
	PreviousLocation = CurrentLocation;
	CurrentLocation = currentLocation;

	PreviousLocation.Z = 0;
	CurrentLocation.Z = 0;

	float dist = FVector::Dist(CurrentLocation, PreviousLocation);

	TotalMovedDistance += dist;
}
