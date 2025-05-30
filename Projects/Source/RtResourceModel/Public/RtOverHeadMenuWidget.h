// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RtObjectActorComponent.h"

#include "RtOverHeadMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTRESOURCEMODEL_API URtOverHeadMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	bool IsValidObjectActorComponent()
	{
		return (nullptr != ObjectActorComponent);
	}

	UFUNCTION(BlueprintCallable)
	void XR_SelectNextMenuLoop();

	//UFUNCTION(BlueprintCallable)
	//void XR_DoExecuteSelectMenu();

	UFUNCTION(BlueprintCallable)
	void SetCurrentMenuMaxCount(int32 maxCount)
	{
		CurrentMenuMaxCount = maxCount;
	}

	UFUNCTION(BlueprintCallable)
	void SetInitSelectMenuIndex()
	{
		XR_SelectMenuIndex = 1;
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URtObjectActorComponent* ObjectActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XR_SelectMenuIndex = 1; // 0은 선택 없음, 1 ~ 8 까지 선택 상태.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentMenuMaxCount;
};
