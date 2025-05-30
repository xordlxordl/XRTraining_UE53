// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/WidgetComponent.h>
#include "RtEnums.h"
#include "RtObjectOptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTRESOURCEMODEL_API URtObjectOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URtObjectOptionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FString GetObjectKeyName() const { return ObjectKeyName; }
	void SetObjectKeyName(FString val);
	void SetReferenceInfo(FString modelKey, ERtTctType tctType, ERtTctType2 tctType2, int32 index);

	void SetupComponents();
	void SetOverHeadTextColor(FLinearColor InColorAndOpacity);
	void ShowOverHeadText(bool showOverHeadText);

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UWidgetComponent> UIWidgetComponent;

protected:

	// ��ü�� �ĺ��� ���� Ű �̸� ( ������ AActor �� name ���� �Ẹ�� ������ �̰��� 
	// �𸮾� �ý��ۿ��� ���� �Ǵ� �༮�̶� ������ ĳ������ ���� ������� ����� �����ϴ� ���� 
	// �������� ���� �̽��� �־ ������ Ű���� ������ ������ �����Ѵ� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ObjectKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReferenceTctModelKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERtTctType ReferenceTctType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERtTctType2 ReferenceTctType2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReferenceIndex;

};
