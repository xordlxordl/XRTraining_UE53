// Fill out your copyright notice in the Description page of Project Settings.
// UI Manager 
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"
#include "Common/RtBlueprintAssetPath.h"

#include "RtGameHUD.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtHUD, Log, All);

// Framework
class ARtPlayerController;

// Manager
class URtGameManager;
class URtTableManager;

// Widget
class URt_PageBase;
class URt_PrimaryLayout;

/**
 * 
 */
UCLASS()
class RTGAME_API ARtGameHUD : public AHUD
{
	GENERATED_BODY()
	
// Various
private:
	TWeakObjectPtr<URt_PrimaryLayout> Primary_Layout;

// Function
public: 

// Custom Get Set
// Get
   ARtPlayerController* Get_PlayerController();
   URtGameManager* Get_GameManager();
   URtTableManager* Get_TableManager();


// UI Control
   bool Init_Primary_Layer();
   void Open_HUD_Widget();
   URt_PageBase* Open_Page(const FName& InName, bool InPopup);
   bool Close_Popup(URt_PageBase* InPage);
   void Show_MessageBox(const FName& InNmae, FRtDele_MessagResult ResultCallback);

   URt_PageBase* Open_Page_Loading();
   URt_PageBase* LoadingWidget;
   bool Close_Popup_Loading();

// Main Flow
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

};
