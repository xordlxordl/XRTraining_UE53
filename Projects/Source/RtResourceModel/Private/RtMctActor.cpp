// Fill out your copyright notice in the Description page of Project Settings.


#include "RtMctActor.h"



// Sets default values
ARtMctActor::ARtMctActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ARtMctActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtMctActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}










/*UStaticMeshComponent* mMeshComp;
mMeshComp = Cast<UStaticMeshComponent>(RootComponent);


UObject* OwnerObject = nullptr;
FString PropertyPath;
FProperty* MaterialProperty = nullptr;
mMeshComp->GetMaterialPropertyPath(0, OwnerObject, PropertyPath, MaterialProperty);

mTemplateData.TemplateMatPath = FPaths::GetBaseFilename(PropertyPath);*/

