// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "Components/SizeBox.h"

// Widget
#include "UI/Rt_PageBase.h"

#include "Rt_PrimaryLayout.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtPrimaryLayout, Log, All);


/**
 * 
 */
UCLASS()
class RTGAME_API URt_PrimaryLayout : public URt_UserWidget
{
	GENERATED_BODY()
// Various
public:
	UPROPERTY(meta = (BindWidget))	USizeBox* Game_Layer;
	UPROPERTY(meta = (BindWidget))	USizeBox* Menu_Layer;
	UPROPERTY(meta = (BindWidget))	USizeBox* Popup_Layer;
	UPROPERTY()	TArray<URt_PageBase*> Popup_Stack;




// Fucntions

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;


public:

    // UI Control
	URt_PageBase* Open_Page(const FName& InName, bool InPopup, APlayerController* InPC, UUserWidget* InConnection);
	bool Close_Popup(URt_PageBase* InWidget);

	URt_PageBase* Inter_Open_Page(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection);
	URt_PageBase* Inter_Open_Popup(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection);

private:
	void Inter_Change_PageWidget(UUserWidget* InWidget);
	void Inter_Add_PopupWidget(URt_PageBase* InWidget);

};
