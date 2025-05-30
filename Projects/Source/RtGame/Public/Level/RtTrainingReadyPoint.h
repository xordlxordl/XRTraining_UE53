// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// UE
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "RtTrainingReadyPoint.generated.h"

class UTextRenderComponent;
class ARtCharacter;

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogStartPoint, Log, All);

UENUM()
enum class ERtStartPoint_State {
	Visible_Unoccupied,
	Visible_Occupied
};

UCLASS()
class RTGAME_API ARtTrainingReadyPoint : public AActor
{
	GENERATED_UCLASS_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	int32 Index = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TObjectPtr<class UDecalComponent> DecalComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TObjectPtr<class UTextRenderComponent> TextComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TObjectPtr<class UCapsuleComponent>	CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FLinearColor DecalNormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FLinearColor DecalEffectColor;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> DecalMaterial;

public:

	UPROPERTY(ReplicatedUsing = OnRep_ChangedState) ERtStartPoint_State State = ERtStartPoint_State::Visible_Unoccupied;
	UPROPERTY(ReplicatedUsing = OnRep_IsValidSPoint) bool IsValidSPoint = false;// = ERsStartTargetPoint_State::Hide;

	UFUNCTION()	void OnRep_IsValidSPoint();
	UFUNCTION()	void OnRep_ChangedState();


public:	
	// Sets default values for this actor's properties
	ARtTrainingReadyPoint();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 Get_Index();
	void Show_TargetPoint(bool InParam);
	void Set_Text(const FString& InStr);

	void Set_IsValidSPoint(bool InBool);

	void DisableActor(bool toHide);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	

	void Change_State(ERtStartPoint_State InState);

private:
	void Inter_Change_State(ERtStartPoint_State InState);

};
