// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Rt_PageBase.h"

//Framework

#include "Framework/RtPlayerController.h"

void URt_PageBase::Close_Page()
{
// Blank
}

URt_PageBase* URt_PageBase::BPCall_Open_Popup(const FName& InName)
{
	return Open_Popup(InName);
}

bool URt_PageBase::BPCall_Close_Popup(URt_PageBase* InPage)
{
	return Close_Popup(InPage);
}

URt_PageBase* URt_PageBase::BPCall_Open_Page(const FName& InName)
{
	return Open_Page(InName);
}

URt_PageBase* URt_PageBase::Open_Popup(const FName& InName)
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Open_Popup(InName);
	}
	return nullptr;
}

bool URt_PageBase::Close_Popup(URt_PageBase* InPage)
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Close_Popup(InPage);
	}
	return false;
}

URt_PageBase* URt_PageBase::Open_Page(const FName& InName)
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Open_Page(InName);
	}
	return nullptr;
}

void URt_PageBase::Deactivate_Widget()
{
	Close_Popup(this);
}

void URt_PageBase::Post_OpenPage()
{

}
