// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RtEnums.h"

#include "RtTctStructActor.generated.h"

//USTRUCT(BlueprintInternalUseOnly)
//struct FRsTableRow_Level : public FTableRowBase
//{
//
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")
//		FText DisplayText;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")
//		FString TravelString;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")
//		TSoftObjectPtr<UTexture2D> PreviewImage;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")
//		TSoftObjectPtr<UWorld> Level;
//};
//


// 이 클래스는 그냥 언리얼 구조체 제작할 공간을 만들기 위해 만든 더미 클래스 이므로 
// 실제 사용은 하지 않는다. ( 다만, 용도가 발견되면 사용할 수도 있다. )
UCLASS()
class RTRESOURCEMODEL_API ARtTctStructActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtTctStructActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
