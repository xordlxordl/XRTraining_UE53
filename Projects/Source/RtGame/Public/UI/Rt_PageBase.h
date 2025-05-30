// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "Components/SizeBox.h"
#include "Rt_PageBase.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_PageBase : public URt_UserWidget
{
	GENERATED_BODY()
	
public:

// Function

	virtual void Close_Page();
	UFUNCTION(BlueprintCallable)	URt_PageBase* BPCall_Open_Popup(const FName& InName);
	UFUNCTION(BlueprintCallable)	bool BPCall_Close_Popup(URt_PageBase* InPage);
	UFUNCTION(BlueprintCallable)	URt_PageBase* BPCall_Open_Page(const FName& InName);

	virtual URt_PageBase* Open_Popup(const FName& InName);
	virtual bool Close_Popup(URt_PageBase* InPage);
	virtual URt_PageBase* Open_Page(const FName& InName);

	void Deactivate_Widget();
	virtual void Post_OpenPage();

};
