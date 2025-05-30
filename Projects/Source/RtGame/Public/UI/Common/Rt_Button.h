// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/Button.h"
#include "Components/TextBlock.h"
// Data
#include "RtCore/public/Common/RtCommon.h"
#include "Rt_Button.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Button : public URt_UserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	// Click Delegate
	DECLARE_DELEGATE_OneParam(FClickDelegate, ERt_MessageResult);

	FClickDelegate Click_Delegate;

	UTextBlock* ButtonTextBlock;
	UButton* ButtonWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;

	UFUNCTION(BlueprintCallable)
	void OnClicked_Button();

	ERt_MessageResult Click_Result = ERt_MessageResult::Unknown;

	void Clear_ClickEvent();

	void Change_Text(const FText& InText);
	void Change_Texture(UObject* InTexture_N, UObject* InTexture_H, UObject* InTexture_C);
};
