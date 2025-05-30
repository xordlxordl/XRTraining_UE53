// Fill out your copyright notice in the Description page of Project Settings.


#include "RtScenarioControllerBase.h"

#include "Kismet/GameplayStatics.h"

#include "RtGlobalActorManager.h"

// Sets default values
ARtScenarioControllerBase::ARtScenarioControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARtScenarioControllerBase::BeginPlay()
{
	Super::BeginPlay();	

	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("ScenarioController");
		gam->ManagerMap.Add({ key, this });
	}

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Show.DropboxCommands"), TEXT(""),
		FConsoleCommandDelegate::CreateLambda([&](void) {
			if (ScenarioModel.IsValid()) ScenarioModel->Show_DropboxCommands();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Show.LoopEventList"), TEXT(""),
		FConsoleCommandDelegate::CreateLambda([&](void) {
			if (ScenarioModel.IsValid()) ScenarioModel->Show_LoopEventList();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Show.ServerEventCommands"), TEXT(""),
		FConsoleCommandDelegate::CreateLambda([&](void) {
			if (ScenarioModel.IsValid()) ScenarioModel->Show_ServerEventCommands();
			}));

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Show.SuddenEventCommands"), TEXT(""),
		FConsoleCommandDelegate::CreateLambda([&](void) {
			if (ScenarioModel.IsValid()) ScenarioModel->Show_SuddenEventCommands();
			}));
}

void ARtScenarioControllerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

TArray<FString> ARtScenarioControllerBase::ParseCommaString(FString strData)
{
	TArray<FString> valueList;

	int32 commaIndex;
	if (strData.FindChar(',', commaIndex))
	{
		strData.ParseIntoArray(valueList, TEXT(","), true);
	}
	else
	{
		valueList.Add(strData);
	}

	return valueList;
}

// Called every frame
void ARtScenarioControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ARtScenarioControllerBase::LoadSctOnlyDataStruct(FString sctFilePath)
{
	FActorSpawnParameters spawnParams;
	FRotator spawnRotator = FRotator::ZeroRotator;
	FVector spawnLocation = FVector::ZeroVector;

	if (ScenarioModel.Get()) {
		ScenarioModel->Destroy();
	}

	ScenarioModel = GetWorld()->SpawnActor<ARtScenarioModel>(
		ARtScenarioModel::StaticClass(), spawnLocation, spawnRotator, spawnParams);

	bool loadOk = false;
	if (nullptr != ScenarioModel)
	{
		return ScenarioModel->LoadScenarioDataStruct(sctFilePath);
	}

	return false;
}

bool ARtScenarioControllerBase::LoadSCTData(FString sctFilePath)
{
	FActorSpawnParameters spawnParams;
	FRotator spawnRotator = FRotator::ZeroRotator;
	FVector spawnLocation = FVector::ZeroVector;

	if (ScenarioModel.Get())
	{
		ScenarioModel->Destroy();
	}

	UWorld* pWorld = GetWorld();
	ScenarioModel = pWorld->SpawnActor<ARtScenarioModel>(
		ARtScenarioModel::StaticClass(), spawnLocation, spawnRotator, spawnParams);

	bool loadOk = false;
	if (nullptr != ScenarioModel)
	{
		loadOk = ScenarioModel->LoadSCT(sctFilePath);

		if (loadOk)
		{

		}
	}

	return loadOk;
}

bool ARtScenarioControllerBase::SetSceneManagerRef(ARtSceneManagerBase* pSceneManager)
{
	bool bOk = false;
	if (pSceneManager)
	{
		SceneManagerRef = pSceneManager;
		bOk = true;
	}
	else 
	{
		if( GetSceneManagerRef() )
		{
			bOk = true;
		}
	}

	if (bOk)
	{
		GetSceneManagerRef()->SetScenarioControllerRef(this);
	}

	return bOk;
}

ARtSceneManagerBase* ARtScenarioControllerBase::GetSceneManagerRef()
{
	return ARtSceneManagerBase::Get(GetWorld());
}

void ARtScenarioControllerBase::UnloadSct()
{
	if (ScenarioModel.Get())
	{
		ScenarioModel = nullptr;
		//MainObjectMap.Empty();
		MyScenarioPlayer = FRtScenarioPlayer();
	}
}

ARtScenarioControllerBase* ARtScenarioControllerBase::Get(UObject* worldContext)
{
	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(worldContext)->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("ScenarioController");

		if (AActor* findActor = gam->ManagerMap.FindRef(key)) {
			if( IsValid(findActor) )
				return Cast<ARtScenarioControllerBase>(findActor);
		}
		if (AActor* findActor = UGameplayStatics::GetActorOfClass(worldContext, ARtScenarioControllerBase::StaticClass())) {
			if (IsValid(findActor))
				return Cast<ARtScenarioControllerBase>(findActor);
		}
	}

	return nullptr;
}
