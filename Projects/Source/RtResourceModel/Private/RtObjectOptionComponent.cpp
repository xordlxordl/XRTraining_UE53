// Fill out your copyright notice in the Description page of Project Settings.


#include "RtObjectOptionComponent.h"

#if WITH_EDITOR
#include "RtObjectActor.h"
#endif

// Sets default values for this component's properties
URtObjectOptionComponent::URtObjectOptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

//	UIWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UIWidget"));

	// ...
}

// Called when the game starts
void URtObjectOptionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	ReferenceTctType = ERtTctType::ENone;
	ReferenceTctType2 = ERtTctType2::ENone;
}

// Called every frame
void URtObjectOptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URtObjectOptionComponent::SetObjectKeyName(FString val)
{
	ObjectKeyName = val;

#if WITH_EDITOR
	AActor* parentActor = GetOwner();
	if (parentActor) {
		ARtObjectActor* objectActor = Cast<ARtObjectActor>(parentActor);
		if (objectActor) {
			FRtTctStruct* tctStruct = objectActor->GetTctStruct();
			if (tctStruct) {
				FString newName = FString::Format(TEXT("ObjectActor_{0}"), { ObjectKeyName });
				parentActor->SetActorLabel(newName);
			}
		}
	}
#endif

}

void URtObjectOptionComponent::SetReferenceInfo(FString modelKey, ERtTctType tctType, ERtTctType2 tctType2, int32 index)
{
	ReferenceTctModelKey = modelKey;
	ReferenceTctType = tctType;
	ReferenceTctType2 = tctType2;
	ReferenceIndex = index;
}

void URtObjectOptionComponent::SetupComponents()
{
}

void URtObjectOptionComponent::SetOverHeadTextColor(FLinearColor InColorAndOpacity)
{
}

void URtObjectOptionComponent::ShowOverHeadText(bool showOverHeadText)
{
}
