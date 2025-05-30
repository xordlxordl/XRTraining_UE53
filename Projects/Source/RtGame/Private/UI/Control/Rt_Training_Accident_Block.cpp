// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Training_Accident_Block.h"
#include "Kismet/KismetTextLibrary.h"
// Framework
#include "Framework/RtPlayerController.h"
// Network
#include "Network/RtNetworkManager.h"

void URt_Training_Accident_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AccidentBtn_var) {
		AccidentBtn_var->OnClicked.AddDynamic(this, &URt_Training_Accident_Block::OnClicked_Accident);
	}


}
void URt_Training_Accident_Block::OnClicked_Accident()
{
	

	// ��ư ���ٲٴ� ����..? 
	selectedAccident = UKismetTextLibrary::Conv_TextToString(Accident_var->GetText());
	if (parentWidget) {
		parentWidget->SelectedAccident_str = selectedAccident;
		parentWidget->CheckSelected(this);
	}
}


