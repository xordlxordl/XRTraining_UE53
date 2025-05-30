// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "Rt_Trainee_Text.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Trainee_Text : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_var;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DrawSize = FVector2D(1000.0f, 1000.0f);

public:
	UFUNCTION(BlueprintCallable)
	void SetText(const FText& InText);
	UFUNCTION(BlueprintCallable)
	void SetFontSize(const int32 InFontSize = 60);
	UFUNCTION(BlueprintCallable)
	void SetFontColor(const FSlateColor InTextColor);
	UFUNCTION(BlueprintCallable)
	void SetBorderColor(const FLinearColor InBorderColor = FLinearColor(0, 0, 0, 0));

	FORCEINLINE FVector2D GetDrawSize() { return DrawSize; }
	FORCEINLINE FText GetText() { return Text_var->GetText(); }
};
