// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_SoldierRadio.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Framework/RtWidgetActor_Radio.h"

void URt_Trainee_SoldierRadio::NativeConstruct()
{
	if (CircleChargeMaterial) {
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CircleChargeMaterial, this);
		CircleChargeBrush.SetResourceObject(DynamicMaterial);
	}

	Update(ESoldierRadioState::ENone);
}

void URt_Trainee_SoldierRadio::Update(const ESoldierRadioState& NewState, float NewPercent)
{
	switch (NewState)
	{
	case ESoldierRadioState::ENone:
	case ESoldierRadioState::ESelect:
		Image_Radio->SetVisibility(ESlateVisibility::Collapsed);
		Image_Progress->SetVisibility(ESlateVisibility::Collapsed);
		Image_Fail->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case ESoldierRadioState::ECommunicationSuccess:
		Image_Radio->SetVisibility(ESlateVisibility::Visible);
		Image_Progress->SetVisibility(ESlateVisibility::Visible);
		Image_Fail->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case ESoldierRadioState::ECommunicationFail:
		Image_Radio->SetVisibility(ESlateVisibility::Visible);
		Image_Progress->SetVisibility(ESlateVisibility::Collapsed);
		Image_Fail->SetVisibility(ESlateVisibility::Visible);
		break;
	}

	SetPercent(NewPercent);
}

void URt_Trainee_SoldierRadio::SetPercent(float NewPercent)
{
	UObject* Resource = CircleChargeBrush.GetResourceObject();

	// If we already have a dynamic material, return it.
	if (UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource)) {
		DynamicMaterial->SetScalarParameterValue(FName("Percent"), NewPercent);
	}
}
