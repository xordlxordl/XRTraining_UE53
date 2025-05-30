// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_SelectionMenu.h"
#include "UI/Trainee/Rt_Trainee_Selection.h"
#include "Framework/RtWidgetActor.h"
#include "Manager/RtGameScenarioController.h"

#include "Common/RtBlueprintAssetPath.h"

void URt_Trainee_SelectionMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	TArray<UWidget*> children = Scroll_var->GetAllChildren();
	if (children.IsEmpty())
		return;

	for (UWidget* child : children)
	{
		if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(child))
		{
			selection->SetFontSize(FontSize);
			selection->SetBoxHeight(BoxHeight);
			selection->SetSelectColor(SelectColor);
			selection->SetNotSelectColor(NotSelectColor);
			selection->SetNotSelect();
		}
	}

	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[SelectionNum]))
	{
		selection->SetSelect();

		float h = selection->GetDesiredSize().Y;
		int N = children.Num();
		Scroll_var->SetScrollOffset(SelectionNum * (N * h - SizeBox_var->GetHeightOverride()) / (N - 1));
	}
}

void URt_Trainee_SelectionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void URt_Trainee_SelectionMenu::Init()
{
	SelectionNum = 0;

	if (Scroll_var)
		Scroll_var->ClearChildren();
}

void URt_Trainee_SelectionMenu::SelectUp()
{
	if (SelectionNum <= 0)
		return;
	TArray<UWidget*> children = Scroll_var->GetAllChildren();
	uint8 prevSelectedNum = SelectionNum;
	SelectionNum--;
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[prevSelectedNum]))
	{
		selection->SetNotSelect();
	}
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[SelectionNum]))
	{
		selection->SetSelect();

		float h = selection->GetDesiredSize().Y;
		int N = children.Num();
		Scroll_var->SetScrollOffset(SelectionNum * (N * h - SizeBox_var->GetHeightOverride()) / (N - 1));
	}
}

void URt_Trainee_SelectionMenu::SelectDown()
{
	TArray<UWidget*> children = Scroll_var->GetAllChildren();
	if (SelectionNum + 1 >= children.Num())
		return;
	uint8 prevSelectedNum = SelectionNum;
	SelectionNum++;
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[prevSelectedNum]))
	{
		selection->SetNotSelect();
	}
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[SelectionNum]))
	{
		selection->SetSelect();

		float h = selection->GetDesiredSize().Y;
		int N = children.Num();
		Scroll_var->SetScrollOffset(SelectionNum * (N * h - SizeBox_var->GetHeightOverride()) / (N - 1));
	}
}

URt_Trainee_Selection* URt_Trainee_SelectionMenu::AddSelection(FString InEventID, FString InEventName)
{
	TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, RtTraineeWidgetBlueprint::TraineeSelectionWidget);
	if (widgetClass == nullptr)
		return nullptr;

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
	URt_Trainee_Selection* newSelection = Cast<URt_Trainee_Selection>(widget);
	if(newSelection == nullptr)
		return nullptr;

	Scroll_var->AddChild(newSelection);
	newSelection->SetText(FText::FromString(InEventName));
	newSelection->SetEventID(InEventID);

	newSelection->SetFontSize(FontSize);
	newSelection->SetBoxHeight(BoxHeight);
	newSelection->SetSelectColor(SelectColor);
	newSelection->SetNotSelectColor(NotSelectColor);
	newSelection->SetNotSelect();

	TArray<UWidget*> children = Scroll_var->GetAllChildren();
	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[SelectionNum]))
	{
		selection->SetSelect();
	}

	return newSelection;
}

void URt_Trainee_SelectionMenu::ExecuteSelectEvent()
{
	//if (Owner_RtWidgetActor.IsValid() == false)
	//{
	//	Owner_RtWidgetActor->DestroyWidgetActor();
	//	return;
	//}

	TArray<UWidget*> children = Scroll_var->GetAllChildren();
	if (children.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu is Empty"));
		if (Owner_RtWidgetActor.IsValid())
			Owner_RtWidgetActor->DestroyWidgetActor();
		return;
	}

	if (URt_Trainee_Selection* selection = Cast<URt_Trainee_Selection>(children[SelectionNum]))
	{
		if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld()))
		{
			if (gsc->CurrentPlayType != ERt_PlayType::E_Single) {
				if (gsc->bIsGamePlay == false) {
					if (Owner_RtWidgetActor.IsValid())
						Owner_RtWidgetActor->DestroyWidgetActor();
					return;
				}
				selection->SendToServer();
				if (Owner_RtWidgetActor.IsValid())
					Owner_RtWidgetActor->DestroyWidgetActor();
			}
			else {
				gsc->ExecuteServerEvent(selection->GetEventID());
			}
		}

	}
}

void URt_Trainee_SelectionMenu::CancleMenu()
{
	if (Owner_RtWidgetActor.IsValid())
		Owner_RtWidgetActor->DestroyWidgetActor();
}

void URt_Trainee_SelectionMenu::SetOwner_WidgetActor(ARtWidgetActor* InActor)
{
	Owner_RtWidgetActor = TWeakObjectPtr<ARtWidgetActor>(InActor);
}

TArray<UWidget*> URt_Trainee_SelectionMenu::GetSelections()
{
	return Scroll_var->GetAllChildren();
}
