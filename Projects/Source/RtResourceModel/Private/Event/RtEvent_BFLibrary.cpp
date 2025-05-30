// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/RtEvent_BFLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

URtEvent_BFLibrary::Dele_StringCommand URtEvent_BFLibrary::String_Command_Delegate;
//URtEvent_BFLibrary::Dele_StringCommand URtEvent_BFLibrary::UI_Open_Popup_Delegate;
//URtEvent_BFLibrary::Dele_StringCommand URtEvent_BFLibrary::UI_Open_Page_Delegate;
URtEvent_BFLibrary::Dele_SingleCommand URtEvent_BFLibrary::Exit_Game_Delegate;
URtEvent_BFLibrary::Dele_SingleCommand URtEvent_BFLibrary::Start_Training_Delegate;


//URtEvent_BFLibrary::Dele_NameCommand URtEvent_BFLibrary::UI_Open_Popup_Name_Delegate;
//URtEvent_BFLibrary::Dele_NameCommand URtEvent_BFLibrary::UI_Open_Page_Name_Delegate;

URtEvent_BFLibrary::Dele_UI_OpenWidget URtEvent_BFLibrary::UI_OpenWidget_Delegate;
URtEvent_BFLibrary::Dele_UI_CloseWidget URtEvent_BFLibrary::UI_CloseWidget_Delegate;


int32 URtEvent_BFLibrary::String_Command(const FString& InStr,APlayerController* InPc)
{
	if (String_Command_Delegate.IsBound()) {
		return String_Command_Delegate.Execute(InStr, InPc);
	}
	return 0;
}

//int32 URtEvent_BFLibrary::UI_Open_Page(const FString& InStr, APlayerController* InPc)
//{
//	if (UI_Open_Page_Delegate.IsBound()) {
//		return UI_Open_Page_Delegate.Execute(InStr, InPc);
//	}
//	return 0;
//}
//
//
//int32 URtEvent_BFLibrary::UI_Open_Popup(const FString& InStr, APlayerController* InPc, UWidget* InConnection)
//{
//	int32 res = 0;
//	if (UI_Open_Popup_Delegate.IsBound()) {
//		res = UI_Open_Popup_Delegate.Execute(InStr, InPc);
//		if (res == 0) {
//			//if(InPc)
//			//	UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
//		}
//	}
//	return res;
//}

int32 URtEvent_BFLibrary::Exit_Game( APlayerController* InPc)
{
	int32 res = 0;
	if (Exit_Game_Delegate.IsBound()) {
		res = Exit_Game_Delegate.Execute(InPc);
		if (res == 0) {
			//if(InPc)
			//	UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
		}
	}
	return res;
}
int32 URtEvent_BFLibrary::Start_Training(APlayerController* InPc)
{
	int32 res = 0;
	if (Start_Training_Delegate.IsBound()) {
		res = Start_Training_Delegate.Execute(InPc);
		if (res == 0) {
			//if(InPc)
			//	UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
		}
	}
	return res;
}

//
//int32 URtEvent_BFLibrary::UI_Open_Page_Name(const FName& InName, APlayerController* InPc, UWidget* InConnection)
//{
//	if (UI_Open_Page_Name_Delegate.IsBound()) {
//		return UI_Open_Page_Name_Delegate.Execute(InName, InPc, InConnection);
//	}
//	return 0;
//}
//
//
//int32 URtEvent_BFLibrary::UI_Open_Popup_Name(const FName& InName, bool InPopup, APlayerController* InPc, UWidget* InConnection)
//{
//	int32 res = 0;
//	if (UI_OpenWidget_Delegate.IsBound()) {
//		res = UI_OpenWidget_Delegate.Execute(InName, InPopup, InPc, InConnection);
//		if (res == 0) {
//			//if(InPc)
//			//UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
//		}
//	}
//	return res;
//}

UUserWidget* URtEvent_BFLibrary::UI_OpenWidget(const FName& InName, bool InPopup, APlayerController* InPc, UUserWidget* InConnection)
{
	UUserWidget* res = 0;
	if (UI_OpenWidget_Delegate.IsBound()) {
		res = UI_OpenWidget_Delegate.Execute(InName, InPopup, InPc, InConnection);
		if (res == 0) {
			//if(InPc)
			//UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
		}
	}
	return res;
}

bool URtEvent_BFLibrary::UI_CloseWidget(const FName& InName, bool InPopup)//, APlayerController* InPc, UUserWidget* InConnection)
{
	bool res = false;
	if (UI_CloseWidget_Delegate.IsBound()) {
		res = UI_CloseWidget_Delegate.Execute(InName, InPopup);//, InPc, InConnection);
		if (res == 0) {
			//if(InPc)
			//UWidgetLayoutLibrary::RemoveAllWidgets(InPc->GetWorld());
		}
	}
	return res;
}