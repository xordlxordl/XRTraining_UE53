// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RtEvent_BFLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RTRESOURCEMODEL_API URtEvent_BFLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_RetVal_OneParam(int32, Dele_SingleCommand, APlayerController*)
	static Dele_SingleCommand Exit_Game_Delegate;
	static Dele_SingleCommand Start_Training_Delegate;

	DECLARE_DELEGATE_RetVal_TwoParams(int32, Dele_StringCommand, const FString&, APlayerController*)
	static Dele_StringCommand String_Command_Delegate;
	/*static Dele_StringCommand UI_Open_Popup_Delegate;
	static Dele_StringCommand UI_Open_Page_Delegate;*/

	DECLARE_DELEGATE_RetVal_FourParams(UUserWidget*, Dele_UI_OpenWidget, const FName&, bool, APlayerController*, UUserWidget* )
		static Dele_UI_OpenWidget UI_OpenWidget_Delegate;

	DECLARE_DELEGATE_RetVal_TwoParams(bool, Dele_UI_CloseWidget, const FName&, bool)//, APlayerController*, UUserWidget*)
		static Dele_UI_CloseWidget UI_CloseWidget_Delegate;

	//DECLARE_DELEGATE_RetVal_ThreeParams(int32, Dele_NameCommand, const FName&, APlayerController*, UWidget* )
	//	static Dele_UI_OpenWidget UI_Open_Page_Name_Delegate;

	UFUNCTION(BlueprintCallable, Category = "Rs|Ex", meta = (DisplayNmae = "String Command"))
		static int32 String_Command(const FString& str, APlayerController* InPc);
	//UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta = (DisplayNmae = "Open Popup"))
	//	static int32 UI_Open_Popup(const FString& InStr ,APlayerController* InPc, UWidget* InConnection);
	//UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta=(DisplayNmae="Open_Page"))
	//	static int32 UI_Open_Page(const FString& InStr, APlayerController* InPc);

	UFUNCTION(BlueprintCallable, Category = "Rs|Ex|Game", meta = (DisplayNmae = "Exit Game"))
		static int32 Exit_Game(APlayerController* InPc);
	UFUNCTION(BlueprintCallable, Category = "Rs|Ex|Game", meta = (DisplayNmae = "Start Trainging"))
		static int32 Start_Training(APlayerController* InPc);


	//UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta = (DisplayNmae = "Open Popup"))
	//	static int32 UI_Open_Popup_Name(const FName& InName, APlayerController* InPc, UWidget* InConnection);
	//UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta = (DisplayNmae = "Open_Page"))
	//	static int32 UI_Open_Page_Name(const FName& InName, APlayerController* InPc, UWidget* InConnection);

	UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta = (DisplayNmae = "Open Widget"))
	static UUserWidget* UI_OpenWidget(const FName& InName, bool InPopup, APlayerController* InPc, UUserWidget* InConnection);

	UFUNCTION(BlueprintCallable, Category = "Rs|Ex|UI", meta = (DisplayNmae = "Close Widget"))
	static bool UI_CloseWidget(const FName& InName, bool InPopup);//, APlayerController* InPc, UUserWidget* InConnection);
};
