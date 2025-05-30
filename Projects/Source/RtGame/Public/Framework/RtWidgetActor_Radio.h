// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtWidgetActor_Menu.h"
#include "RtWidgetActor_Radio.generated.h"

UENUM(BlueprintType)
enum class ESoldierRadioState : uint8 {
	ENone = 0,
	ETryOpen,
	ETryClose,
	ESelect,
	ECommunicationSuccess,
	ECommunicationFail
};

DECLARE_DELEGATE(FExecuteSystemOption);

/**
 * 
 */

UCLASS()
class RTGAME_API ARtWidgetActor_RadioOption : public ARtWidgetActor_MenuOption
{
	GENERATED_BODY()

public:
	ARtWidgetActor_RadioOption();

public:
	void PostExecute(class ARtWidgetActor_Menu* Menu) override;
};

UCLASS()
class RTGAME_API ARtWidgetActor_SystemOption : public ARtWidgetActor_MenuOption
{
	GENERATED_BODY()

public:
	ARtWidgetActor_SystemOption();

public:
	void Execute() override;

public:
	FExecuteSystemOption ExecuteSystemOption;

};

UCLASS()
class RTGAME_API ARtWidgetActor_Radio : public ARtWidgetActor_Menu
{
	GENERATED_BODY()
	
public:
	ARtWidgetActor_Radio();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SystemRoot;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ESoldierRadioState RadioState = ESoldierRadioState::ENone;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ESoldierRadioState PrevRadioState = ESoldierRadioState::ENone;

	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	float CurTime = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	float MaxGrapTime = 1.5f;
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	FSlateColor SystemOptionColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	FSlateColor SuddenOptionColor = FLinearColor(1.0f, 0.2f, 0.0f, 1.0f);
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	int32 Column = 6;
	
public:
	void RadioInit();
	void SystemInit(int8& LeftOptionNum, int8& RightOptionNum);

	void CreateRadioOption(int32 Index, FString EventName, FString EventID, FSlateColor Color = FColor::White);
	ARtWidgetActor_SystemOption* CreateSystemOption(FString EventName, int8& LeftOptionNum, int8& RightOptionNum, FSlateColor Color, bool IsRight = false);
	void CreateSuddenOption(FString EventName, FString EventID, int8& LeftOptionNum, int8& RightOptionNum, FSlateColor Color);

	void SetSystemOption(ARtWidgetActor_SystemOption* MenuOption, FString EventName, FSlateColor Color);
	void SetBoxPosition(ARtWidgetActor_MenuOption* MenuOption, int8& LeftOptionNum, int8& RightOptionNum, bool IsRight = false);

	void TryOpen();
	void TryClose(bool IsImmediate = false);
	void Update();
	void Cancle();

	//void ShowOptions() override;
	//void HideOptions() override;

	void TryCommunication();
	void CommunicationSuccess();
	void CommunicationFail();

};
