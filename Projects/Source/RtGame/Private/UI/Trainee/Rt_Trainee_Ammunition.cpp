// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_Ammunition.h"

void URt_Trainee_Ammunition::SetAmmunitionType(bool isLive)
{
	Switcher_Ammo->SetActiveWidgetIndex(isLive ? 1 : 0);

}
