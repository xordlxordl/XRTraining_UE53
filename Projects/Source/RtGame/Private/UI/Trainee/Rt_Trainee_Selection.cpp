// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_Selection.h"
#include "Kismet/GameplayStatics.h"

// Network
#include "Network/RtNetworkManager.h"

void URt_Trainee_Selection::NativePreConstruct()
{
	SetNotSelect();
}

void URt_Trainee_Selection::SetText(const FText& InText)
{
	if (Text_var) {
		Text_var->SetText(InText);
	}
}

void URt_Trainee_Selection::SetFontSize(const int32& InFontSize)
{
	if (Text_var) {
		FSlateFontInfo font = Text_var->GetFont();
		font.Size = InFontSize;
		Text_var->SetFont(font);
	}
}

void URt_Trainee_Selection::SetBoxHeight(const float& InBoxHeight)
{
	if (SizeBox_var) {
		SizeBox_var->SetHeightOverride(InBoxHeight);
	}
}

void URt_Trainee_Selection::SetSelectColor(const FLinearColor& InColor)
{
	SelectColor = InColor;
}

void URt_Trainee_Selection::SetNotSelectColor(const FLinearColor& InColor)
{
	NotSelectColor = InColor;
}

void URt_Trainee_Selection::SetEventID(const FString& InEventID)
{
	EventID = InEventID;
}

void URt_Trainee_Selection::SetSelect()
{
	if (Border_var)
	{
		Border_var->SetBrushColor(SelectColor);
	}
}

void URt_Trainee_Selection::SetNotSelect()
{
	if (Border_var)
	{
		Border_var->SetBrushColor(NotSelectColor);
	}
}

void URt_Trainee_Selection::SendToServer()
{
	// EventID¸¦ Server¿¡ º¸³¿
	if (auto* nmgr = GetNetworkManager()) {
		nmgr->SendPacket_SCTEventID(EventID);
	}
}
