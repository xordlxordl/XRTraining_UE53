// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_Main.h"
#include "UI/Trainee/Rt_Trainee_SelectionMenu.h"
#include "UI/Trainee/Rt_Trainee_Selection.h"
#include "UI/Trainee/Rt_Trainee_Ammunition.h"

#include "Misc/MessageDialog.h"

#include "RtDefines.h"
#include "RtBluePrintCommonUtil.h"

#include "Manager/RtConfig.h"
#include "Manager/RtGameScenarioController.h"
#include "Models/RtScenarioModel.h"

#include "Components/OverlaySlot.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"

void URt_Trainee_Main::SetUsingTrackers(bool isUsing)
{
	TrackersWidget->SetVisibility(isUsing ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void URt_Trainee_Main::UpdateRoleText()
{
	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
	if (gsc == nullptr) {
		Role_Text->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	FString role = gsc->CurrentPlayerID;
	if (role.IsEmpty()) {
		Role_Text->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	UStringTable* TraineeStringTable = LoadObject<UStringTable>(nullptr, TRAINEE_STRINGTABLE_PATH);
	if (TraineeStringTable == nullptr) {
		Role_Text->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Role_Text->SetVisibility(ESlateVisibility::Visible);

	FString OUTPUT;
	TraineeStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RoleText"), OUTPUT);
	OUTPUT = OUTPUT.Replace(TEXT("%s"), *role);
	Role_Text->SetText(FText::FromString(OUTPUT));

}

void URt_Trainee_Main::SetSctText(FString text, FSlateColor color)
{
	SCT_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SCT_Text->SetText(FText::FromString(text));
	SCT_Text->SetColorAndOpacity(color);
}

bool URt_Trainee_Main::RemoveSctText()
{
	if (SCT_Text->GetVisibility() == ESlateVisibility::Hidden)
		return false;
	SCT_Text->SetVisibility(ESlateVisibility::Hidden);
	return true;
}

bool URt_Trainee_Main::HideMenuWidget()
{
	if (CurrentMenu.IsValid()) {
		CurrentMenu.Get()->SetVisibility(ESlateVisibility::Hidden);
		CurrentMenu.Reset();

		return true;
	}

	return false;
}

void URt_Trainee_Main::ShowRadioWidget()
{
	SoldierRadio->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CurrentMenu = MakeWeakObjectPtr<URt_Trainee_SelectionMenu>(SoldierRadio);

}

void URt_Trainee_Main::UpdateRadioMenu()
{
	SoldierRadio->Init();

	ARtScenarioModel* scenarioModel = ARtGameScenarioController::Get(GetWorld())->ScenarioModel.Get();
	FRtRadioDropboxCommand* menuSetCommand = &(scenarioModel->RadioDropboxCommand);
	SoldierRadio->SetMenuSetID(menuSetCommand->DropboxID);

	//Set Menu Commands
	for (auto& data : menuSetCommand->DropboxElemantList)
	{
		URt_Trainee_Selection* selection = SoldierRadio->AddSelection(data.EventID, data.EventCommandName);
		if (selection == nullptr)
		{
			FString errorTitle = FString(TEXT("Radio Event Error"));
			FString errorMsg = FString::Printf(TEXT("%s"), *data.EventID);
			FText errorTitleText = FText::FromString(TEXT("Radio Event Error"));

			UE_LOG(LogTemp, Log, TEXT("%s : %s"), *errorTitle, *errorMsg);
			CUSTOM_FILE_APP_LOG_EX("App.log", "%s : %s", *errorTitle, *errorMsg);
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitleText);

			return;
		}
	}

	ERtSctSystemMenuScreenPos radioPosType = scenarioModel->SctData.RadioDropbox.ScreenPosInfo;

	UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(SoldierRadio->Slot);

	switch (radioPosType)
	{
	case ERtSctSystemMenuScreenPos::ELeftBottom:
		overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
		break;
	case ERtSctSystemMenuScreenPos::ERightBottom:
		overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
		overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
		break;
	case ERtSctSystemMenuScreenPos::ERightTop:
		overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
		overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
		break;
	}

}

bool URt_Trainee_Main::SelectUp()
{
	if (!CurrentMenu.IsValid())
		return false;
	CurrentMenu->SelectUp();
	return true;
}

bool URt_Trainee_Main::SelectDown()
{
	if (!CurrentMenu.IsValid())
		return false;
	CurrentMenu->SelectDown();
	return true;
}

bool URt_Trainee_Main::SelectMenu()
{
	if (!CurrentMenu.IsValid())
		return false;

	CurrentMenu->ExecuteSelectEvent();

	//RecordResultLog
	ARtScenarioModel* scenarioModel = ARtGameScenarioController::Get(GetWorld())->ScenarioModel.Get();
	TArray <UWidget*> selections = CurrentMenu->GetSelections();
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(selections[CurrentMenu->SelectionNum]))
	{
		FString eventID = selection->GetEventID();
		TPair<const FRtEventCommand*, FString> data = scenarioModel->ServerEventCommandMap.FindRef(eventID);
		const FRtEventCommand* commands = data.Key;
		const FString menuSetID = data.Value;

		if (commands == nullptr)
			return true;

		for (const FRtSctFunctionCommand& command : commands->ProcessCommandList)
		{
			const ERtSctFunCmd& cmd = command.FunctionCommand;
			if (cmd == ERtSctFunCmd::RecordResultLog)
			{
				if (auto found = ARtGameScenarioController::Get(this)->CallCommandFunctionMap.Find(cmd))
				{
					bool retValue = false;
					if (found->ExecuteIfBound(command.Params, retValue))
					{
					}
				}
			}
		}
	}

	return true;
}

void URt_Trainee_Main::SetAmmunitionType(bool isLive)
{
	Ammunition->SetAmmunitionType(isLive);
}
