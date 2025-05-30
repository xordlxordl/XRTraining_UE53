// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_Text.h"

void URt_Trainee_Text::SetText(const FText& InText)
{
	if (Text_var) {
		Text_var->SetText(InText);
	}
}

void URt_Trainee_Text::SetFontSize(const int32 InFontSize)
{
	if (Text_var) {
		FSlateFontInfo font = Text_var->GetFont();
		font.Size = InFontSize;
		Text_var->SetFont(font);
	}
}

void URt_Trainee_Text::SetFontColor(const FSlateColor InTextColor)
{
	if (Text_var) {
		Text_var->SetColorAndOpacity(InTextColor);
	}
}

void URt_Trainee_Text::SetBorderColor(const FLinearColor InBorderColor)
{
	if (Border_var) {
		Border_var->SetBrushColor(InBorderColor);
	}
}
