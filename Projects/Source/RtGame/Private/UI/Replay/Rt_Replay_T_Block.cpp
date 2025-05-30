// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Replay/Rt_Replay_T_Block.h"
// Rep Mgr
#include "Manager/RtReplayManager.h"
#include "Manager/RtLoadingManager.h"
// UI
#include "UI/Replay/Rt_ReplayViewer.h"

void URt_Replay_T_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	UGameInstance* gi = GetGameInstance();
	if (gi) {
		RepManager = gi->GetSubsystem<URtReplayManager>();
	}

	Training_Btn->OnClicked.AddDynamic(this, &URt_Replay_T_Block::OnClicked_Btn);

	if (Border_Selected) {
		Border_Selected->BrushColor.A = 0.0f;
	}
}


void URt_Replay_T_Block::SetTrainingName(FString folderName, FString realPath)
{
	int32 index = -1;
	if (realPath.FindLastChar(TEXT('/'), index)) {

		int32 index2 = -1;
		if (folderName.FindLastChar(TEXT('('), index2)) {
			folderName = folderName.Left(index2);
		}

		FString tidInfo = FString::Format(TEXT(" TID : {0}"), { realPath.Mid(index + 1) });
		folderName += tidInfo;
	}
	
	TrainingName->SetText(FText::FromString(folderName));
	RealPath = realPath;

	// Set Replay Info
	FString MakePath = FString::Printf(TEXT("TrainingData/RepFiles/"));


	TArray<FString> Parts;
	TArray<FString> SemiParts;
	RealPath.ParseIntoArray(Parts, TEXT("TrainingData/RepFiles/"), true);

	Parts[1].ParseIntoArray(SemiParts, TEXT("/"), true);

	ScenarioName = SemiParts[0];
	TID = FCString::Atoi(*SemiParts[1]);
}

void URt_Replay_T_Block::SetParentsWidget(URt_ReplayViewer* InParentsWidget)
{
	ParentsWidget = InParentsWidget;
}

void URt_Replay_T_Block::OnClicked_Btn()
{
	// Loading Start
	auto* gi = GetGameInstance();
	if (gi) {
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
			auto* pc = Get_PlayerController();
			if (pc) {
				lm->LoadLevelWithLoadingScreen("Lobby_Map", pc);
			}
		}
	}

	VoiceFilePath = TEXT("");

	// Check Files
	TArray<FString> VoiceList;
	TArray<FString> MotionList;
	IFileManager& FileManager = IFileManager::Get();

	// 폴더 내의 모든 .wav 파일 찾기
	FString FullPath = FPaths::ConvertRelativePathToFull(RealPath);
	FString SearchPattern = FullPath / TEXT("ReplayVoice.wav");
	//FString SearchMotion = FullPath / TEXT("Motion.json");
	IFileManager::Get().FindFiles(VoiceList, *SearchPattern, true, false);
	//IFileManager::Get().FindFiles(MotionList, *SearchMotion, true, false);
	if (VoiceList.Num() == 0)
	{
		// web 에서 download 부터하라는 에러메시지 
	}
	else
	{
		VoiceFilePath = SearchPattern;
		//for (const FString& VoiceFile : VoiceList)
		//{
		//	VoiceFilePath = SearchPattern;
		//}
	}
	
	//if (MotionList.Num() == 0)
	//{
	//	// web 에서 download 부터하라는 에러메시지 
	//}
	//else
	//{
	//	for (const FString& MotionFile : MotionList)
	//	{
	//		MotionFilePath = SearchMotion;
	//	}
	//}

	if (RepManager) {
		RepManager->ClearSCT();
		RepManager->ReplayInfo.SelectedScenarioName = ScenarioName;
		RepManager->ReplayInfo.SelectedTID = TID;
		RepManager->InitManager();

		// Download Code
		if (VoiceFilePath.IsEmpty()) {
			URtWebManager* webManager = gi->GetSubsystem<URtWebManager>();
			if (webManager) {
				webManager->GetVoiceList(TID);
			}
		} else {
			RepManager->SetWavFullPathMap(FullPath);
		}
	}

// Viewer Control

	if (ParentsWidget) {
		if (!VoiceFilePath.IsEmpty()) {
			ParentsWidget->ImportFile(VoiceFilePath);
		}
		ParentsWidget->ReadyHandler_Btn.AddUFunction(this, TEXT("CallLoadSCT"));

		// 선택된 객체 하일라이트.
		if (Border_Selected) {
			ParentsWidget->ClearAllSelectTrainingList();

			SetSelected(true);

			ParentsWidget->Init();
		}
	}
}

void URt_Replay_T_Block::SetSelected(bool selected)
{
	if (Border_Selected) {
		Border_Selected->BrushColor.A = (selected) ? 1.0f : 0.0f;
		Border_Selected->SynchronizeProperties();
	}
}

void URt_Replay_T_Block::CallLoadSCT()
{
	// Load SCT
	if (RepManager) {
		RepManager->LoadSCTDatas();
	}
    // Clear
	ParentsWidget->ReadyHandler_Btn.Clear();
}
