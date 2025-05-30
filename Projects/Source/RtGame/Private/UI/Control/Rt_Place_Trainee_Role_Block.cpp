// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Role_Block.h"

void URt_Place_Trainee_Role_Block::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void URt_Place_Trainee_Role_Block::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void URt_Place_Trainee_Role_Block::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}
