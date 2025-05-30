// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"

#include "Rt_Trainee_Selection.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Trainee_Selection : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_var;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_var;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectColor = FLinearColor(0.5f, 1.0f, 0.5f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor NotSelectColor = FLinearColor(0.5f, 0.5f, 0.2f, 1.0f);

	UPROPERTY(EditInstanceOnly)
	FString EventID;

public:
	void SetText(const FText& InText);
	void SetFontSize(const int32& InFontSize);
	void SetBoxHeight(const float& InBoxHeight);
	void SetSelectColor(const FLinearColor& InColor);
	void SetNotSelectColor(const FLinearColor& InColor);

	void SetEventID(const FString& InEventID);
	FORCEINLINE const FString& GetEventID() { return EventID; }

	void SetSelect();
	void SetNotSelect();

	void SendToServer();
};
