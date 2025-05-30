// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Rt_MessageBox.h"
// Framework
#include "Framework/RtPlayerController.h"
// Data
#include "Common/RtDataTableRows.h"
// UI 
#include "UI/Common/Rt_Button.h"

void URt_MessageBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Tap_var) {
		Tap_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void URt_MessageBox::Close_Dialog()
{
	if (auto* pc = Get_PlayerController()) {
		pc->Close_Popup(this);
	}
}

FEventReply URt_MessageBox::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Popup(this);
	return FEventReply(false);
}

void URt_MessageBox::CloseConfirmationWindow(ERt_MessageResult Result)
{
	Deactivate_Widget();
	OnResultCallback.ExecuteIfBound(Result);
}

void URt_MessageBox::Setup_Dialog(FRtTableRow_MessageBox* InInfo, FRtDele_MessagResult ResultCallback)
{
	Title_var->SetText(InInfo->Header);
	Description_var->SetText(InInfo->Body);

	Buttons_var->Reset<UButton>([](UButton& Button)
		{
			/////////////	Button.OnClicked().Clear();
		});

	for (const auto& ButtonInfo : InInfo->ButtonArray)
	{
		FDataTableRowHandle ActionRow;
		URt_Button* ButtonWidget = Buttons_var->CreateEntry<URt_Button>();

		switch (ButtonInfo.Result)
		{
		case ERt_MessageResult::Confirmed:
			break;
		case ERt_MessageResult::Declined:
			break;
		case ERt_MessageResult::Cancelled:
			if (ButtonWidget->ButtonWidget)
			ButtonWidget->Change_Texture(InInfo->Texture_Normal, InInfo->Texture_Hover, InInfo->Texture_Cancel);
			break;
		default:
			continue;
		}


		if (ButtonWidget->ButtonWidget) {
			ButtonWidget->Click_Delegate.BindUObject(this, &ThisClass::CloseConfirmationWindow);
			ButtonWidget->Click_Result = ButtonInfo.Result;
			ButtonWidget->Change_Text(ButtonInfo.Text);
			
		}
	}
	OnResultCallback = ResultCallback;
}
