// Fill out your copyright notice in the Description page of Project Settings.

#include "RtOverHeadMenuWidget.h"

void URtOverHeadMenuWidget::XR_SelectNextMenuLoop()
{
	XR_SelectMenuIndex++;
	if (XR_SelectMenuIndex > CurrentMenuMaxCount)
	{
		XR_SelectMenuIndex = 1;
	}
}
