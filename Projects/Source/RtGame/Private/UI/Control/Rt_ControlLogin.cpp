// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Control/Rt_ControlLogin.h"
// UE
#include "Kismet/GameplayStatics.h"

// Network
#include "Network/RtNetworkManager.h"
// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtWebManager.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_ControlLogin::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Login_Btn) {
		Login_Btn->OnClicked.AddDynamic(this, &URt_ControlLogin::OnClicked_Login);
	}
	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	ErrorMsgBoxTable = LoadObject<UStringTable>(nullptr, *URtConfig::ErrorMsgPath);
	if (ControlStringTable == nullptr) return;
	if (ErrorMsgBoxTable == nullptr) return;
	if (Account_text && Password_text && Errorbox_text && Login_text && Quit_text) { 

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Account"), OUTPUT);
		Account_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Password"), OUTPUT);
		Password_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Login"), OUTPUT);
		Login_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
	}

	// dave
	//check(Password_var);
	
	UserID_var->SetText(FText::FromString(TEXT("")));
	Password_var->SetText(FText::FromString(TEXT("")));

#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	UserID_var->SetText(FText::FromString(TEXT("tool")));
	Password_var->SetText(FText::FromString(TEXT("*****")));
#endif
#endif

	Password_var->SetIsPassword(false);
	InPass.Empty();
	bIsUpdatingText = false;

	Password_var->OnTextChanged.AddDynamic(this, &URt_ControlLogin::OnPasswordTextChanged);
}

// dave
void URt_ControlLogin::NativeConstruct()
{
	Super::NativeConstruct();

	if (UserID_var)
	{
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(UserID_var->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		UserID_var->SetKeyboardFocus();
	}
}

/*
*  현재 별 다른 에러 메시지가 없어 일괄 적용
*/
void URt_ControlLogin::CheckLogin(int32 Result)
{
	FString OUTPUT;

	switch (Result)
	{
	case (int32)ERtWebProtocolType::webErrorServerCheck:
		if (!Errorbox_text) {
			return;
		}
		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));
		break;
	case (int32)ERtWebProtocolType::webErrorBlock:
		if (!Errorbox_text) {
			return;
		}
		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));
		break;
	case (int32)ERtWebProtocolType::WebError:
		if (!Errorbox_text) {
			return;
		}
		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));
		break;
	case (int32)ERtWebProtocolType::webErrorDup:
		if (!Errorbox_text) {
			return;
		}
		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));
		break;
	case (int32)ERtWebProtocolType::webErrorParse:
		if (!Errorbox_text) {
			return;
		}
		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));
		break;
	case 1:                                                //Login Success!! 
		BPCall_Open_Page("ControlSelectMap");
		break;
	default:
		break;
	}
}

void URt_ControlLogin::OnClicked_Login()
{
	// Connect Web
	auto* WebSubsys = GetRtWebManager();
	if (WebSubsys->IsInitialized())
	{
		FString InId;
		FText UserID = UserID_var->GetText();
		InId = UserID.ToString();

#ifdef DAVE_PASSWORD_VISIBLE
		FString InPass;
		FText Password = Password_var->GetText();
		InPass = Password.ToString();
#endif

#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
		InPass = "1234";
#endif
#endif
		WebSubsys->Login(InId, InPass);
		WebSubsys->Handle_LoginCheck.AddUFunction(this, FName("CheckLogin"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Invalid Web Subsystem !! "));
		return;
	}
}

// dave
UFUNCTION()
void URt_ControlLogin::OnPasswordTextChanged(const FText& InText)
{
	if (bIsUpdatingText)
		return;

	bIsUpdatingText = true;

	FString CurrentText = InText.ToString();
	int32 CurrentLength = CurrentText.Len();
	int32 PrevLength = InPass.Len();

	if (CurrentLength > PrevLength)
	{
		InPass += CurrentText.Right(CurrentLength - PrevLength);
	}
	else if (CurrentLength < PrevLength)
	{
		InPass = InPass.Left(CurrentLength);
	}

	FString MaskedText = FString::ChrN(InPass.Len(), TEXT('*'));
	Password_var->SetText(FText::FromString(MaskedText));

	bIsUpdatingText = false;
}

void URt_ControlLogin::OpenNextPage()
{
	BPCall_Open_Page("ControlSelectMap");
}
