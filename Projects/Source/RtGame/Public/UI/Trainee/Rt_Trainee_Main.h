// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "UI/Trainee/Rt_Trainee_SelectionMenu.h"
#include "UI/Trainee/Rt_Trainee_Ammunition.h"

#include "Components/TextBlock.h"

#include "Rt_Trainee_Main.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Trainee_Main : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	URt_UserWidget* TrackersWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Role_Text;

	UPROPERTY(meta = (BindWidget))
	URt_Trainee_SelectionMenu* SoldierRadio;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SCT_Text;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* System_Text;
	
	UPROPERTY(meta = (BindWidget))
	URt_Trainee_Ammunition* Ammunition;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DrawSize = FVector2D(1000.0f, 1000.0f);

public:
	void SetUsingTrackers(bool isUsing);

	void UpdateRoleText();
	void SetSctText(FString text, FSlateColor color = FLinearColor::White);
	bool RemoveSctText();

	bool HideMenuWidget();

	void ShowRadioWidget();
	void UpdateRadioMenu();

	bool SelectUp();
	bool SelectDown();
	bool SelectMenu();

	void SetAmmunitionType(bool isLive);
	
	FORCEINLINE FVector2D GetDrawSize() { return DrawSize; }

protected:
	TWeakObjectPtr<URt_Trainee_SelectionMenu> CurrentMenu;

};
