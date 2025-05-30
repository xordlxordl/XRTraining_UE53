// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/Button.h"
// Data
#include "RtCore/public/Common/RtCommon.h"
#include "Rt_MessageBox.generated.h"

struct FRtTableRow_MessageBox;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_MessageBox : public URt_PageBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	void Close_Dialog();

	UPROPERTY(meta = (BindWidget))
	UBorder* Tap_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Title_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<URichTextBlock> Description_var;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> Buttons_var;

protected:
	UFUNCTION()
	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);

public:

	void CloseConfirmationWindow(ERt_MessageResult Result);

	FRtDele_MessagResult OnResultCallback;

	void Setup_Dialog(FRtTableRow_MessageBox* InInfo, FRtDele_MessagResult ResultCallback);

};
