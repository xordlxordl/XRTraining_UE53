// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "Rt_UserWidget.generated.h"

#define RSDEF_UI_EVENT_BINDING_CLICKED( p_value, p_delegate )\
if(p_value && !p_value->OnClicked.IsBound() ) p_value->OnClicked.AddDynamic(this, &p_delegate);

class URtGameManager;
class URtNetworkManager;
class URtWebManager;
class URtTrainingManager;
class ARtPlayerController;
class ARtGameState;
class ARtGameHUD;
class ARtGameMode;
class URt_ListViewObject_ScenarioInfo;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_UserWidget : public UUserWidget
{
	GENERATED_BODY()

// Various
public:
	TWeakObjectPtr<UUserWidget> Connection_Widget;
	FName Widget_Name = "";

// Functions
public:
// Main Flow
	virtual void Begin_Play(); // use virtual only
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

// Custom Get Set
// Get
    UFUNCTION(BlueprintCallable)
	URtGameManager* GetGameManager();
	UFUNCTION(BlueprintCallable)
	URtNetworkManager* GetNetworkManager();
	UFUNCTION(BlueprintCallable)
	URtWebManager* GetRtWebManager();

	UFUNCTION(BlueprintCallable)
	URtTrainingManager* GetTrainingManager();

	UFUNCTION(BlueprintCallable)
	ARtPlayerController* Get_PlayerController();
	UFUNCTION(BlueprintCallable)
	ARtGameState* Get_GameState();
	UFUNCTION(BlueprintCallable)
	ARtGameMode* Get_GameMode();
	UFUNCTION(BlueprintCallable)
	ARtGameHUD* Get_HUD();


// PlayerCheck
	UFUNCTION(BlueprintCallable)
	bool IsControl_PlayType();
	bool IsLocalPlayer();
	bool IsLocalConllerPlayType();


// Control Widget
	bool BPCall_Quit();
	
	//virtual void Handle_C_Event(const FName& Name, const FString& String); // handle the event
	//virtual void Handle_B_Event(const FName& Name, const FString& String); // handle the event
	//
	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Trigger C Event"))
	//	void Trigger_C_Event(const FName& Name, const FString& String);  // Trigger an event in cpp
	//UFUNCTION(BlueprintNativeEvent)
	//	void Trigger_B_Event(const FName& Name, const FString& String); // Trigger an event in blueprint

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

};
