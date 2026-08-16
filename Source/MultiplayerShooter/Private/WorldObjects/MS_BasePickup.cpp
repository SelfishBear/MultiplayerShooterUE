// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/MS_BasePickup.h"
#include "Components/BoxComponent.h"

AMS_BasePickup::AMS_BasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxCollision->SetupAttachment(PickupMesh);

	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AMS_BasePickup::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
}

void AMS_BasePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Animate(DeltaSeconds);
}

void AMS_BasePickup::Pickup(ACharacter* PickingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup!"));
	OnPickUp();
}

void AMS_BasePickup::OnPickUp()
{
	PlayPickUpCosmetics();
	SetActorHiddenInGame(true);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, this, &AMS_BasePickup::OnRestored, RestoreDelay);
}

void AMS_BasePickup::OnRestored()
{
	SetActorHiddenInGame(false);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMS_BasePickup::Animate(float DeltaTime)
{
	AnimateLocation();
	AnimateRotation(DeltaTime);
}

void AMS_BasePickup::AnimateRotation(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0.0f, AnimationSettings.RotationSpeed * DeltaTime, 0.0f));
}

void AMS_BasePickup::AnimateLocation()
{
	const float ZOffset = (AnimationSettings.Amplitude * FMath::Sin(
		AnimationSettings.Frequency * GetWorld()->GetTimeSeconds()));

	SetActorLocation(InitialLocation + FVector(0.0, 0.0, ZOffset));
}
