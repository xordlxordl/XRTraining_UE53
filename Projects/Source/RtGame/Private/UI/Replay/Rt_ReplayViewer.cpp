// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Replay/Rt_ReplayViewer.h"

#include "HAL/FileManager.h"
#include "Misc/Paths.h"

#include "Kismet/GameplayStatics.h"
// Manager 
#include "Manager/RtReplayManager.h"
#include "Manager/RtReplayManagerActor.h"

// Comp
#include "Components/AudioComponent.h"

// RAI
#include "RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioExporter.h"
#include "RuntimeAudioImporter.h"

#include "Camera/CameraActor.h"
#include "Common/RtGameplayTags.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "UI/Replay/Rt_Replay_T_Block.h"
#include "UI/Replay/Rt_Replay_U_Block.h"
#include "Components/CanvasPanelSlot.h"
#include <Blueprint/WidgetLayoutLibrary.h>

#include "AudioDevice.h"

void URt_ReplayViewer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Quit_Btn_var) {
		Quit_Btn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Quit);
	}

	if (Sound_Btn_var) {
		Sound_Btn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Volume);
	}

	if (Sound_X_Btn_var) {
		Sound_X_Btn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Volume);
	}

	if (PlayBtn_var) {
		PlayBtn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Play);
	}

	if (PauseBtn_var) {
		PauseBtn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Pause);
	}

	if (ReplayBtn_var) {
		ReplayBtn_var->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_Replay);
	}

	if (ReplaySlider_var) {
		ReplaySlider_var->OnValueChanged.AddUniqueDynamic(this, &URt_ReplayViewer::OnSliderValueChanged);
		
	}

	if (SoundVolumeSlider_var) {
		SoundVolumeSlider_var->OnValueChanged.AddUniqueDynamic(this, &URt_ReplayViewer::OnSoundVolumeSliderValueChanged);
	}

	if( B_TopView ) {
		B_TopView->OnClicked.AddUniqueDynamic(this, &URt_ReplayViewer::OnClicked_TopView);
	}

	MixedAudioComp = NewObject<UAudioComponent>(GetWorld());

	if (auto* gi = GetGameInstance()) {
		RepMgr = gi->GetSubsystem<URtReplayManager>();
		if (RepMgr){
			UE_LOG(LogTemp, Warning, TEXT("Success Get Replay Manager"));
			RepMgr->SetCompHandler.AddUFunction(this, TEXT("ImportFile"));
			RepMgr->userHandler.AddUFunction(this, TEXT("Finish_GetUserList"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Fail Get Replay Manager"));
		}
	}

	FString SavedFilesPath;

#if WITH_EDITOR
	SavedFilesPath = FPaths::ProjectDir();
#else
	SavedFilesPath = FPaths::LaunchDir();
#endif

	SavedFilesPath = FPaths::Combine(SavedFilesPath, TEXT("TrainingData/RepFiles/"));

	TrainingFolderList = GetFolderList(SavedFilesPath);
	if (TrainingFolderList.IsEmpty()) return;
	BPEvent_Update_T_Blocks();

	if (ARtReplayManagerActor::Get(GetWorld())) {
		ARtReplayManagerActor::Get(GetWorld())->ReplayEndEvent.BindUFunction(this, "OnReplayEnd");
	}

	if (MixedAudioComp && SoundVolumeSlider_var)
	{
		SoundVolumeSlider_var->SetValue(MixedAudioComp->VolumeMultiplier);
	}
}

void URt_ReplayViewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ImportedSoundWave)
	{
		if (ImportedSoundWave->IsPlaying(GetWorld()) && ReplaySlider_var) {

			if (bIsPause) {

			}
			else {
				CurrentPlaybackTime = ImportedSoundWave->GetPlaybackTime();
				TotalDuration = ARtReplayManagerActor::Get(GetWorld())->TotalReplayTime;// ImportedSoundWave->GetDuration();

				SliderValue = CurrentPlaybackTime / TotalDuration;

				ReplaySlider_var->SetValue(SliderValue);
				ARtReplayManagerActor::Get(GetWorld())->Replaying(CurrentPlaybackTime);
			}
		}
	}
}

void URt_ReplayViewer::OnClicked_Quit()
{
	// Quit Game
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}

void URt_ReplayViewer::OnClicked_Volume()
{
	if (SoundVolumeSlider_var)
	{
		if (SoundVolumeSlider_var->GetVisibility() == ESlateVisibility::Hidden) {
			SoundVolumeSlider_var->SetVisibility(ESlateVisibility::Visible);

			if (MixedAudioComp)
			{
				SoundVolumeSlider_var->SetValue(MixedAudioComp->VolumeMultiplier);
			}
		}
		else {
			SoundVolumeSlider_var->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URt_ReplayViewer::OnClicked_PlaySub()
{
	if (ARtReplayManagerActor::Get(GetWorld())) {
		ARtReplayManagerActor::Get(GetWorld())->PlayReplay();

		float replayInterval = ARtReplayManagerActor::Get(GetWorld())->ReplayInterval;

		GetWorld()->GetTimerManager().ClearTimer(ReplayTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ReplayTimerHandle,
			FTimerDelegate::CreateUObject(this, &URt_ReplayViewer::ReplayProgress), replayInterval, true, 0.0f);

		float totalReplayTime = ARtReplayManagerActor::Get(GetWorld())->TotalReplayTime;
		float currentReplayTime = ARtReplayManagerActor::Get(GetWorld())->CurrentReplayTimeRate * totalReplayTime;

		SetTotalTime(totalReplayTime);
		SetCurrentTime(currentReplayTime);
	}
}

void URt_ReplayViewer::OnClicked_Play()
{
	if (MixedAudioComp) {
		// Switch Pause Button
		BPEvent_ControlButton_Switch(1);
		MixedAudioComp->Play();

		OnClicked_PlaySub();
	}
}

void URt_ReplayViewer::ReplayProgress()
{
	if (ReplaySlider_var) {
		float currentReplayTimeRate = ARtReplayManagerActor::Get(GetWorld())->CurrentReplayTimeRate;
		if (currentReplayTimeRate < 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(ReplayTimerHandle);
			ReplaySlider_var->SetValue(0);
			SetCurrentTime(0);
			return;
		}

		ReplaySlider_var->SetValue(currentReplayTimeRate);

		float totalReplayTime = ARtReplayManagerActor::Get(GetWorld())->TotalReplayTime;
		float currentReplayTime = ARtReplayManagerActor::Get(GetWorld())->CurrentReplayTimeRate * totalReplayTime;
		SetCurrentTime(currentReplayTime);
	}
}

void URt_ReplayViewer::OnClicked_Pause()
{
	bIsPause = true;

	if (MixedAudioComp) {
		BPEvent_ControlButton_Switch(2);
		if (PlayBtn_var) {
			PlayBtn_var->OnClicked.Clear();
		}

		if (ARtReplayManagerActor::Get(GetWorld())) {
			ARtReplayManagerActor::Get(GetWorld())->PauseReplay();
		}

		if (MixedAudioComp->IsPlaying()) {
			// Switch Play Button
			MixedAudioComp->SetPaused(true);
		}
	}
}

void URt_ReplayViewer::OnClicked_Replay()
{
	bIsPause = false;

	if (MixedAudioComp) {
		// Switch Pause Button
		BPEvent_ControlButton_Switch(1);

		if (MixedAudioComp->IsPlaying()) {
			if (MixedAudioComp->bIsPaused) {
				MixedAudioComp->SetPaused(false);
			}
		} else {
			MixedAudioComp->Play();
		}
//		MixedAudioComp->SetPaused(false);

		OnClicked_PlaySub();
	}
}

void URt_ReplayViewer::OnReplayEnd()
{
	bIsPause = false;

	if (MixedAudioComp) {
		BPEvent_ControlButton_Switch(2);
		if (PlayBtn_var) {
			PlayBtn_var->OnClicked.Clear();
		}

		if (MixedAudioComp->IsPlaying()) {
			MixedAudioComp->Stop();
		}
	}
}

void URt_ReplayViewer::OnSliderValueChanged(float Value)
{
	if (ARtReplayManagerActor::Get(GetWorld())) {
		ARtReplayManagerActor::Get(GetWorld())->SetCurrentPlayTime(Value);

		if (ImportedSoundWave) {
			float PlayTime = Value * ImportedSoundWave->GetDuration();
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("Playing Time : %f"), PlayTime));
			ImportedSoundWave->RewindPlaybackTime(PlayTime);

			if (ReplaySlider_var)
			{
				CurrentPlaybackTime = ImportedSoundWave->GetPlaybackTime();
				TotalDuration = ARtReplayManagerActor::Get(GetWorld())->TotalReplayTime;// ImportedSoundWave->GetDuration();

				SliderValue = CurrentPlaybackTime / TotalDuration;
				ReplaySlider_var->SetValue(SliderValue);
			}
		}
	}
}

void URt_ReplayViewer::OnSoundVolumeSliderValueChanged(float Value)
{
	if (SoundVolumeSlider_var && MixedAudioComp)
	{
		bool volumeOn = Value > 0.0f;

		if (Value == 0.0f) {
			// 0으로 만들면 플레이 중에는 다시 값을 올려도 소리가 안나와서..
			Value = 0.001f;
		}

		MixedAudioComp->SetVolumeMultiplier(Value);

		if (SB_SoundVolume && SB_SoundVolumeX) {
			SB_SoundVolume->SetVisibility(volumeOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			SB_SoundVolumeX->SetVisibility(volumeOn ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
}

void URt_ReplayViewer::Init()
{
	OnClicked_TopView();

	ReplaySlider_var->SetValue(0);

	GetWorld()->GetTimerManager().ClearTimer(ReplayTimerHandle);

	bIsPause = false;

	BPEvent_ControlButton_Switch(2);

	if (ARtReplayManagerActor::Get(GetWorld())) {
		ARtReplayManagerActor::Get(GetWorld())->CurrentReplayTimeRate = -1.0f;
	}

	SetCurrentTime(0);
	SetTotalTime(0);
}

void URt_ReplayViewer::SetTotalTime(float totalTime)
{
	if (T_TotalTime) {
		T_TotalTime->SetText(FText::FromString(FString::Format(TEXT("{0}"), { totalTime })));
	}
}

void URt_ReplayViewer::SetCurrentTime(float currentTime)
{
	if (T_CurrentTime) {
		T_CurrentTime->SetText(FText::FromString(FString::Format(TEXT("{0}"), { currentTime })));
	}
}

void URt_ReplayViewer::OnClicked_TopView()
{
	// 1. 모든 리플레이 훈련 플레이어가 렌더타겟에 카메라 뷰 쓰기를 못하게 막는다.
	// 2. 기존의 탑뷰 카메라 액터가 카메라 뷰 쓰기를 하게 만든다.
	if (RepMgr)
	{
		OnClicked_Pause();

		RepMgr->ClearAllTraineeCaptureTextureTarget();

		ACameraActor* camera = nullptr;
		TArray<AActor*> OutActor;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), RtPlayTags::RtReplayMode_Actor_Camera, OutActor);
		for (auto* Actor : OutActor)
		{
			camera = Cast<ACameraActor>(Actor);
			if (camera) {

				USceneCaptureComponent2D* sceneCaptureCom = Cast<USceneCaptureComponent2D>(
					camera->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
				
				FString RenderTargetPath = TEXT("/Game/RS/Control/Camera/ReplayView_RT.ReplayView_RT");
				UTextureRenderTarget2D* LoadedRenderTarget = Cast<UTextureRenderTarget2D>(
					StaticLoadObject(UTextureRenderTarget2D::StaticClass(), nullptr, *RenderTargetPath));

				if (sceneCaptureCom && LoadedRenderTarget) {
					sceneCaptureCom->TextureTarget = LoadedRenderTarget;
				}

				ClearAllSelectTraineeList();

				SetTopViewButtonSelected(true);

				ChangeWavFileTopView();
				return;
			}
		}

	}

}


void URt_ReplayViewer::OpenDialogReplay()
{

}

TArray<FString> URt_ReplayViewer::GetFolderList(const FString& Directory)
{
	TArray<FString> FolderPathList;
	TArray<FString> FolderList;
	IFileManager& FileManager = IFileManager::Get();

	FileManager.IterateDirectory(*Directory, [&FolderList, &FolderPathList](const TCHAR* FilenameOrDirectory, bool bIsDirectory) {
		if (bIsDirectory)
		{
			FolderPathList.Add(FilenameOrDirectory);
			//FolderList.Add(FPaths::GetCleanFilename(FilenameOrDirectory));
		}
		return true; // 계속 탐색
	});

	for (const FString& Folder : FolderPathList)
	{
		FString FolderPath = Folder;
		FString FolderName = FPaths::GetCleanFilename(Folder);
		TArray<FString> TIDFolderList;
		// TID 개수에 따라 중복된 이름 표기

		FileManager.IterateDirectory(*FolderPath, [&TIDFolderList](const TCHAR* TIDDirectory, bool bIsTIDDirectory) {
			if (bIsTIDDirectory)
			{
				TIDFolderList.Add(FPaths::GetCleanFilename(TIDDirectory));
			}
			return true; // 계속 탐색
		});

		for (int i = 0; i < TIDFolderList.Num(); i++) {
			FString TotalFolderName;
			FString TotalFolderPath;
			if (i == 0) {
				TotalFolderName = FolderName;
			}
			else {
				TotalFolderName = FString::Printf(TEXT("%s(%d)"), *FolderName, i);
			}
			TotalFolderPath = FPaths::Combine(FolderPath, TIDFolderList[i]);
			FolderList.Add(TotalFolderName);
			FilePathMap.Add(TotalFolderName, TotalFolderPath);
		}
	}
	return FolderList;
}

void URt_ReplayViewer::SetMixedAudioComp(USoundWave* InValue)
{
	MixedAudioComp->SetSound(InValue);
}

void URt_ReplayViewer::ImportFile(const FString& FilePath)
{
	// Import Buffer
	URuntimeAudioImporterLibrary* Importer = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
	Importer->OnResult.AddDynamic(this, &URt_ReplayViewer::Finish_Import);
	Importer->ImportAudioFromFile(FilePath, ERuntimeAudioFormat::Wav);
}

void URt_ReplayViewer::ChangeWavFileTopView()
{
	if (RepMgr && MixedAudioComp) {
		//MixedAudioComp->Stop();
		if (!RepMgr->PlayedFilePath.IsEmpty()) {
			ImportFile(RepMgr->PlayedFilePath);
		}
	}
}

void URt_ReplayViewer::ChangeWavFileByUID(int32 uid)
{
	if (RepMgr && MixedAudioComp) {
		//MixedAudioComp->Stop();
		FString* wavFullPath = RepMgr->PlayerReplayWavFullPathMap.Find(uid);
		if (wavFullPath) {
			ImportFile(*wavFullPath);
		}
	}
}

void URt_ReplayViewer::Finish_GetUserList()
{
	// Web Comunication Done
	if (!RepMgr) return;
	TraineeNameList = RepMgr->ReplayInfo.GetNames();
	TraineeRoleList = RepMgr->ReplayInfo.GetRoles();
	TraineeUIDList  = RepMgr->ReplayInfo.GetUIDs();
	BPEvent_Update_U_Blocks();

	/*
	* IsBound = TRUE  : Bind Function From T_Block
	* IsBound = FALSE : Not Bind. First Execute Replay 
	*/
	if (ReadyHandler_Btn.IsBound()) {
		ReadyHandler_Btn.Broadcast();
	}

}

void URt_ReplayViewer::Finish_Import(URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWav, ERuntimeImportStatus Status)
{
	bool isOldPlaying = MixedAudioComp->IsPlaying();

	ImportedSoundWave = ImportedSoundWav;
	SetMixedAudioComp(ImportedSoundWave);

	if (ImportedSoundWave && isOldPlaying) {

		GetWorld()->GetTimerManager().ClearTimer(TempTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			TempTimerHandle,             // 타이머 핸들
			FTimerDelegate::CreateLambda([this]()
				{
					float value = 0;
					if (ARtReplayManagerActor::Get(GetWorld())) {
						value = CurrentPlaybackTime / ARtReplayManagerActor::Get(GetWorld())->TotalReplayTime;
					}
					OnSliderValueChanged(value);
				}),
			1.0f,                    // 딜레이 시간 (초)
			false                    // 반복 여부 (false = 1회만 실행)
		);
	}
}

std::string URt_ReplayViewer::GetBase64Decording(std::string encoded_string)
{

	const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4] = { 0, }, char_array_3[3] = { 0, };

	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && IsBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;

		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i]; i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

bool URt_ReplayViewer::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

void URt_ReplayViewer::SetTopViewButtonSelected(bool selected)
{
	if (Border_TopView) {
		Border_TopView->BrushColor.A = (selected) ? 1.0f : 0.0f;
		Border_TopView->SynchronizeProperties();
	}
}

void URt_ReplayViewer::ClearAllSelectTrainingList()
{
	if (T_Stacks)
	{
		for (int32 i = 0; i < T_Stacks->GetChildrenCount(); i++) {
			URt_Replay_T_Block* TID_ButtonWidget = Cast<URt_Replay_T_Block>(T_Stacks->GetChildAt(i));
			if (TID_ButtonWidget) {
				TID_ButtonWidget->SetSelected(false);
			}
		}
	}
}

void URt_ReplayViewer::ClearAllSelectTraineeList()
{
	SetTopViewButtonSelected(false);

	if (U_Stacks)
	{
		for (int32 i = 0; i < U_Stacks->GetChildrenCount(); i++) {
			URt_Replay_U_Block* Trainee_ButtonWidget = Cast<URt_Replay_U_Block>(U_Stacks->GetChildAt(i));
			if (Trainee_ButtonWidget) {
				Trainee_ButtonWidget->SetSelected(false);
			}
		}
	}
}


