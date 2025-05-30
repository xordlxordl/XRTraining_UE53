// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Rt_Button.h"

void URt_Button::NativePreConstruct()
{
	Super::NativePreConstruct();

	//`` text block
	FName textblock_name = FName("Text_var");
	if (auto* textblock_widget = Cast<UTextBlock>(GetWidgetFromName(textblock_name))) {
		ButtonTextBlock = textblock_widget;
		ButtonTextBlock->SetText(ButtonText);
	}

	//`` button event
	FName button_name = FName("Button_var");
	if (auto* button_widget = Cast<UButton>(GetWidgetFromName(button_name))) {
		ButtonWidget = button_widget;
		ButtonWidget->OnClicked.AddUniqueDynamic(this, &URt_Button::OnClicked_Button);
	}
}

void URt_Button::OnClicked_Button()
{
	if (Click_Delegate.IsBound()) {
		Click_Delegate.Execute(Click_Result);
	}

	Clear_ClickEvent();
}

void URt_Button::NativeConstruct()
{
	Super::NativeConstruct();
}

void URt_Button::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URt_Button::Clear_ClickEvent()
{
	if (ButtonWidget) {
		ButtonWidget->OnClicked.Clear();
	}
}

void URt_Button::Change_Text(const FText& InText)
{
	if (ButtonTextBlock) {
		ButtonText = InText;
		ButtonTextBlock->SetText(ButtonText);
	}
}

void URt_Button::Change_Texture(UObject* InTexture_N, UObject* InTexture_H, UObject* InTexture_C)
{
	if (ButtonWidget) {
		FButtonStyle newstyle;
		FSlateBrush newBrush_N;
		newBrush_N.SetResourceObject(InTexture_N);
		FSlateBrush newBrush_H;
		newBrush_H.SetResourceObject(InTexture_H);
		FSlateBrush newBrush_C;
		newBrush_C.SetResourceObject(InTexture_C);

		newstyle.SetNormal(newBrush_N);
		newstyle.SetHovered(newBrush_H);
		newstyle.SetPressed(newBrush_C);

		ButtonWidget->SetStyle(newstyle);
	}
}
