// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE UI
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Sizebox.h"
#include "Components/Slider.h"
#include "Components/StackBox.h"
#include "Components/CanvasPanel.h"
#include "Rt_ReplayViewer.generated.h"

/**
 * Replay Viewer 
 * 1. Download Files from WebServer 
 * 2. Tied files to one folder 
 * 3. Init Datas 
 * 4. Play Replay files 
 * 5. additional function : stop, pause, play 
 */
class UAudioComponent;
class URtReplayManager;

DECLARE_MULTICAST_DELEGATE(FReadyForReplay);


UCLASS()
class RTGAME_API URt_ReplayViewer : public URt_PageBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))	USizeBox* SizeBox_Screen_Var;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UCanvasPanel* Canvas_Panel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UImage* Image_Blind_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Quit_Btn_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Sound_Btn_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Sound_X_Btn_var;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* PlayBtn_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* PauseBtn_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* ReplayBtn_var;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   USlider* ReplaySlider_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   USlider* SoundVolumeSlider_var;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   USizeBox* SB_SoundVolume;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   USizeBox* SB_SoundVolumeX;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UBorder* Border_TopView;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* B_TopView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UStackBox* U_Stacks;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UStackBox* T_Stacks;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* T_CurrentTime;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* T_TotalTime;

	UFUNCTION() void OnClicked_Quit();
	UFUNCTION() void OnClicked_Volume();

	void OnClicked_PlaySub();
	
	UFUNCTION() void OnClicked_Play();
	UFUNCTION() void OnClicked_Pause();
	UFUNCTION() void OnClicked_Replay();

	UFUNCTION() void OnReplayEnd();

	UFUNCTION() void OnSliderValueChanged(float Value);
	UFUNCTION() void OnSoundVolumeSliderValueChanged(float Value);

	UFUNCTION() void Init();

	UFUNCTION() void SetTotalTime(float totalTime);
	UFUNCTION() void SetCurrentTime(float currentTime);

	UFUNCTION() void OnClicked_TopView();

	void ReplayProgress();

	UPROPERTY(BlueprintReadOnly) int32 widgetNum; // 0 : Play, 1 : Pause, 2: Stop 

	UFUNCTION(BlueprintImplementableEvent)	void BPEvent_Update_T_Blocks();
	UFUNCTION(BlueprintImplementableEvent)	void BPEvent_Update_U_Blocks();
	UFUNCTION(BlueprintImplementableEvent)	void BPEvent_ControlButton_Switch(int32 CheckNum);

	

	void OpenDialogReplay();
	TArray<FString> GetFolderList(const FString& Directory);
	FString SeletedFileName;
	bool bIsPause = false;

	// playing control
	float CurrentPlaybackTime;
	float TotalDuration;
	float SliderValue;

	URtReplayManager* RepMgr;
	UPROPERTY() UAudioComponent* MixedAudioComp;
	UFUNCTION() void SetMixedAudioComp(USoundWave* InValue);

	FTimerHandle ReplayTimerHandle;

	FTimerHandle TempTimerHandle;

	// Handler
	/*
	* Must UnBind Rt_Replay_T_Block's Binding Function after Called
	*/
	FReadyForReplay ReadyHandler_Btn;

	// RAI
	UPROPERTY()
	class URuntimeAudioImporterLibrary* RuntimeAudioImporter;
	UFUNCTION()
	void ImportFile(const FString& FilePath);
	UFUNCTION()
	void ChangeWavFileTopView();
	UFUNCTION()
	void ChangeWavFileByUID(int32 uid);

	UFUNCTION()
	void Finish_GetUserList();

	UFUNCTION() 
	void Finish_Import(class URuntimeAudioImporterLibrary* Importer, class UImportedSoundWave* ImportedSoundWav, enum ERuntimeImportStatus Status);
	class UImportedSoundWave* ImportedSoundWave;

	UPROPERTY(BlueprintReadOnly) TArray<FString> TrainingFolderList;
	UPROPERTY(BlueprintReadOnly) TArray<FString> TraineeNameList;
	UPROPERTY(BlueprintReadOnly) TArray<FString> TraineeRoleList;

	UPROPERTY(BlueprintReadOnly) TArray<int32> TraineeUIDList;

	UPROPERTY(BlueprintReadOnly) TMap<FString /*FileName on Widget*/, FString /*RealFilePath*/> FilePathMap;

	void SetTopViewButtonSelected(bool selected);

	void ClearAllSelectTrainingList();
	void ClearAllSelectTraineeList();
	
	UPROPERTY()
	TObjectPtr<UUserWidget> VolumeSliderWidget;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);
};
