// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"

#include "Rt_Trainee_SelectionMenu.generated.h"

/**
 * 
 */
class ARtWidgetActor;

UCLASS()
class RTGAME_API URt_Trainee_SelectionMenu : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* SizeBox_var;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border_var;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* Scroll_var;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 SelectionNum = 0;
	
	void Init();

	UFUNCTION(BlueprintCallable)
	void SelectUp();
	UFUNCTION(BlueprintCallable)
	void SelectDown();

	UFUNCTION(BlueprintCallable)
	class URt_Trainee_Selection* AddSelection(FString InEventID = "", FString InEventName = "");

	UFUNCTION(BlueprintCallable)
	void ExecuteSelectEvent();

	UFUNCTION(BlueprintCallable)
	void CancleMenu();

	void SetOwner_WidgetActor(ARtWidgetActor* InActor);

	FORCEINLINE void SetMenuSetID(const FString& InMenuSetID) { MenuSetID = InMenuSetID; }
	FORCEINLINE FString GetMenuSetID() { return MenuSetID; }
	FORCEINLINE FVector2D GetDrawSize() { return DrawSize; }

	TArray<UWidget*> GetSelections();

protected:
	TWeakObjectPtr<ARtWidgetActor> Owner_RtWidgetActor;
	
	FString MenuSetID = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DrawSize = FVector2D(1000.0f, 1000.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FontSize = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoxHeight = 124.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectColor = FLinearColor(0.5f, 1.0f, 0.5f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor NotSelectColor = FLinearColor(0.5f, 0.5f, 0.2f, 1.0f);
};
