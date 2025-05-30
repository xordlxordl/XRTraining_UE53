// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtListenIconUserWidget.h"
#include "Framework/RtWidgetActor_ListenIcon.h"
#include "Components/TextBlock.h"

bool URtListenIconUserWidget::GetIsListening()
{
    return IsListening;
}

void URtListenIconUserWidget::SetIsListening(bool InIsListening)
{
    IsListening = InIsListening;
}

bool URtListenIconUserWidget::GetPrevIsListening()
{
    return PrevIsListening;
}

void URtListenIconUserWidget::SetPrevIsListening(bool InPrevIsListening)
{
    PrevIsListening = InPrevIsListening;
}

void URtListenIconUserWidget::SetCommandString(const FString& InCommandString)
{
    CommandString = InCommandString;

    if (UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CommandText"))))
    {
        TextBlock->SetText(FText::FromString(CommandString));
    }
}
