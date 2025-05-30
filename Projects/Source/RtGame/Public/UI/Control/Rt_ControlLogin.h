// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Textblock.h"

#include "Rt_ControlLogin.generated.h"

/**
 * 
 */

UCLASS()
class RTGAME_API URt_ControlLogin : public URt_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))	UButton* Login_Btn;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UEditableText* UserID_var;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UEditableText* Password_var;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Account_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Password_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Errorbox_text;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Login_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Quit_text;

	UPROPERTY() UStringTable* ErrorMsgBoxTable;


// Func
	UFUNCTION() void OnClicked_Login();
	UFUNCTION() void OnPasswordTextChanged(const FText& Text);
	UFUNCTION() void OpenNextPage();
	UFUNCTION() void CheckLogin(int32 Result);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	// dave
	bool bIsUpdatingText;
	FString InPass;
};
