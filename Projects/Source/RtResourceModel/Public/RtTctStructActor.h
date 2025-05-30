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


// �� Ŭ������ �׳� �𸮾� ����ü ������ ������ ����� ���� ���� ���� Ŭ���� �̹Ƿ� 
// ���� ����� ���� �ʴ´�. ( �ٸ�, �뵵�� �߰ߵǸ� ����� ���� �ִ�. )
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
