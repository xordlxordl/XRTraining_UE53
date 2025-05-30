// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/Rt_CharacterTopView.h"

void URt_CharacterTopView::NativePreConstruct()
{
	Change_Text(DisplayText);
}

void URt_CharacterTopView::Change_Text(const FText& InText)
{
	DisplayText = InText;
	if (Text_var) {
		Text_var->SetText(InText);
	}
}

void URt_CharacterTopView::ReleaseSlateResources(bool bReleaseChildren)
{
	// dave - prevent restart exception
	if (IsValid(Text_var))
	{
		Text_var->ReleaseSlateResources(true);
	}

	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> URt_CharacterTopView::RebuildWidget()
{
	return Super::RebuildWidget();
}
