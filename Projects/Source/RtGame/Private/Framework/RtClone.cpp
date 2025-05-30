// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtClone.h"
#include "Framework/RtTraineePawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"

// Sets default values
ARtClone::ARtClone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);

	DefaultMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DefaultMesh"));
	DefaultMesh->SetupAttachment(DefaultRoot);
	DefaultMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	//DefaultMesh->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
	DefaultMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	TrackerPelvisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerPelvisMesh"));
	TrackerPelvisMesh->SetupAttachment(DefaultMesh);
	TrackerPelvisMesh->SetCollisionProfileName(FName("NoCollision"));
	
	TrackerLeftFootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerLeftFootMesh"));
	TrackerLeftFootMesh->SetupAttachment(DefaultMesh);
	TrackerLeftFootMesh->SetCollisionProfileName(FName("NoCollision"));
	
	TrackerRightFootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerRightFootMesh"));
	TrackerRightFootMesh->SetupAttachment(DefaultMesh);
	TrackerRightFootMesh->SetCollisionProfileName(FName("NoCollision"));
	
	TrackerLeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerLeftHandMesh"));
	TrackerLeftHandMesh->SetupAttachment(DefaultMesh);
	TrackerLeftHandMesh->SetCollisionProfileName(FName("NoCollision"));
	
	TrackerRightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerRightHandMesh"));
	TrackerRightHandMesh->SetupAttachment(DefaultMesh);
	TrackerRightHandMesh->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void ARtClone::BeginPlay()
{
	Super::BeginPlay();
	
	TraineePawn = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void ARtClone::ShowOption(float Yaw, bool isM)
{
	FVector loc = TraineePawn->Camera->GetComponentLocation() + TraineePawn->Camera->GetForwardVector() * 200.0f;
	loc.Z = 0.0f;
	SetActorLocation(loc);
	FRotator rot = FRotator::ZeroRotator;
	rot.Yaw = Yaw;
	SetActorRotation(rot);

	//DefaultMesh->SetSkeletalMesh(TraineePawn->SkeletalMesh->GetSkeletalMeshAsset());
	DefaultMesh->SetVisibility(true);

	TrackerPelvisMesh->SetVisibility(true);
	TrackerLeftFootMesh->SetVisibility(true);
	TrackerRightFootMesh->SetVisibility(true);
	TrackerLeftHandMesh->SetVisibility(false);
	TrackerRightHandMesh->SetVisibility(false);

	IsMirror = isM;
	FVector scale = TraineePawn->MotionScale;
	if(isM)
		scale.Y *= -1.0f;

	DefaultMesh->SetRelativeScale3D(scale);
}

// Called every frame
void ARtClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TraineePawn)
	{
		FTransform traineeBoneTransform = TraineePawn->SkeletalMesh->GetBoneTransform(0);
		FTransform cloneBoneTransform = DefaultMesh->GetBoneTransform(0);
		
		FTransform applytransform = traineeBoneTransform.Inverse() * cloneBoneTransform;
		
		TrackerPelvisMesh->SetWorldTransform(TraineePawn->TrackerPelvis->GetComponentTransform() * applytransform);		 
		TrackerLeftFootMesh->SetWorldTransform(TraineePawn->TrackerLeftFoot->GetComponentTransform() * applytransform);
		TrackerRightFootMesh->SetWorldTransform(TraineePawn->TrackerRightFoot->GetComponentTransform() * applytransform);
		//TrackerLeftHandMesh->SetWorldTransform(TraineePawn->TrackerLeftHand->GetComponentTransform() * applytransform);
		//TrackerRightHandMesh->SetWorldTransform(TraineePawn->TrackerRightHand->GetComponentTransform() * applytransform);
	}
}

void ARtClone::CalibrationClone()
{
	ShowOption(TraineePawn->Camera->GetComponentRotation().Yaw, false);
}

void ARtClone::ShowClone()
{
	ShowOption(TraineePawn->Camera->GetComponentRotation().Yaw - 90, false);
}

void ARtClone::ShowMirrorClone()
{
	ShowOption(TraineePawn->Camera->GetComponentRotation().Yaw + 90, true);
}

void ARtClone::HideClone()
{
	DefaultMesh->SetVisibility(false);
	TrackerPelvisMesh->SetVisibility(false);
	TrackerLeftFootMesh->SetVisibility(false);
	TrackerRightFootMesh->SetVisibility(false);
	TrackerLeftHandMesh->SetVisibility(false);
	TrackerRightHandMesh->SetVisibility(false);
}

