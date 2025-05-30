// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_MenuOption.h"

void URt_Trainee_MenuOption::NativeConstruct()
{
	if (CircleChargeMaterial) {
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CircleChargeMaterial, this);
		CircleChargeBrush.SetResourceObject(DynamicMaterial);
	}

	SetPercent(0.0f);
}

void URt_Trainee_MenuOption::SetPercent(float NewPercent)
{
	UObject* Resource = CircleChargeBrush.GetResourceObject();

	// If we already have a dynamic material, return it.
	if (UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource)) {
		DynamicMaterial->SetScalarParameterValue(FName("Percent"), NewPercent);
	}
}
