// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Training_Path_Popup.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Training_Path_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (LeaningPath_text && TrainingPath_text && TrainingPathView_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("PathOfTrainee"), OUTPUT);
		TrainingPath_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ViewOfPath"), OUTPUT);
		TrainingPathView_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeepPath"), OUTPUT);
		LeaningPath_text->SetText(FText::FromString(OUTPUT));
	}

}

void URt_Training_Path_Popup::NativeDestruct()
{
	Super::NativeDestruct();
}

FEventReply URt_Training_Path_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Training_Path_Popup::Close_Page()
{
	Close_Popup(this);
}