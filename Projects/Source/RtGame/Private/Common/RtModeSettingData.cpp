// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/RtModeSettingData.h"

FName URtModeSettingData::Get_UI(ERt_PlayType InPlayType)
{
	switch (InPlayType)
	{
	case ERt_PlayType::E_Trainee:
		return TrainerMode.UI_Name;
	case ERt_PlayType::E_Replay:
		return ReplayMode.UI_Name;
	case ERt_PlayType::E_Control:
		return ControlMode.UI_Name;
	case ERt_PlayType::E_Server:
		return ServerMode.UI_Name;
	default:
		return NAME_None;
	}
}

const FRtModeInfo& URtModeSettingData::Get_Mode(ERt_PlayType InPlayType)
{
	switch (InPlayType)
	{
	case ERt_PlayType::E_Trainee:
		return TrainerMode;
	case ERt_PlayType::E_Control:
		return ControlMode;
	case ERt_PlayType::E_Server:
		return ServerMode;
	case ERt_PlayType::E_Replay:
		return ReplayMode;
	default:
		return DefaultMode;
	}
}
