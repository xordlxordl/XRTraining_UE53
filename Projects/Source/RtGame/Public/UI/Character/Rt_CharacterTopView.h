// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
// Data
#include "Common/RtRpcCommon.h"
#include "Rt_CharacterTopView.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_CharacterTopView : public URt_UserWidget
{
	GENERATED_BODY()

public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual void NativePreConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* Text_var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayText;
public:
	void Change_Text(const FText& InText);

};
