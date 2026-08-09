// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_CombatComponent.h"

#include "Components/MS_HealthComponent.h"
#include "Core/MS_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UMS_CombatComponent::UMS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UMS_CombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DebugTrace();
}

void UMS_CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMS_CombatComponent, bIsAiming);
}

void UMS_CombatComponent::RequestFire()
{
	if (!GetOwner()) return;

	AMS_PlayerCharacter* Character = Cast<AMS_PlayerCharacter>(GetOwner());
	if (!Character) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

	if (!PlayerController) return;

	int32 ScreenX = 0;
	int32 ScreenY = 0;

	PlayerController->GetViewportSize(ScreenX, ScreenY);

	FVector2D ScreenCenter = FVector2D(ScreenX / 2, ScreenY / 2);

	FVector WorldLocation = FVector::ZeroVector;
	FVector WorldDirection = FVector::ForwardVector;

	PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

	const FVector TraceTarget = WorldLocation + (WorldDirection * TraceDistance);

	if (Character->HasAuthority())
	{
		PerformServerFire(TraceTarget);
	}
	else
	{
		ServerFire(TraceTarget);
	}
}

void UMS_CombatComponent::SetAiming(bool NewIsAiming)
{
	bIsAiming = NewIsAiming;
	ApplyAimingState();
	ServerSetAim(NewIsAiming);
}

void UMS_CombatComponent::ServerSetAim_Implementation(bool bNewIsAiming)
{
	bIsAiming = bNewIsAiming;
	ApplyAimingState();
}

void UMS_CombatComponent::OnRep_IsAiming()
{
	ApplyAimingState();
}

void UMS_CombatComponent::ApplyAimingState()
{
	AMS_PlayerCharacter* Character =
		Cast<AMS_PlayerCharacter>(GetOwner());

	if (!Character) return;

	Character->ApplyAimingMovementSettings();
}

void UMS_CombatComponent::ServerFire_Implementation(FVector_NetQuantize TraceTarget)
{
	PerformServerFire(TraceTarget);
}

void UMS_CombatComponent::PerformServerFire(const FVector& TraceTarget)
{
	AMS_PlayerCharacter* Shooter =
		Cast<AMS_PlayerCharacter>(GetOwner());

	if (!Shooter || !Shooter->HasAuthority()) return;

	AController* Controller = Shooter->GetController();
	if (!Controller) return;

	FVector TraceStart;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(TraceStart, ViewRotation);

	const FVector TraceDirection =
		(TraceTarget - TraceStart).GetSafeNormal();

	const FVector TraceEnd =
		TraceStart + TraceDirection * TraceDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Shooter);
	CollisionParams.bTraceComplex = true;

	FHitResult HitResult;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
	                                                       ECollisionChannel::ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red);

	if (!bHit) return;

	AMS_PlayerCharacter* HitCharacter = Cast<AMS_PlayerCharacter>(HitResult.GetActor());
	if (!HitCharacter) return;

	if (UMS_HealthComponent* HealthComponent = HitCharacter->GetHealthComponent())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Hit Health Comp");
		}
		HealthComponent->RequestHealthChange(-Damage);
	}
}

void UMS_CombatComponent::DebugTrace()
{
	if (!GetOwner()) return;

	AMS_PlayerCharacter* Character = Cast<AMS_PlayerCharacter>(GetOwner());
	if (!Character) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

	if (!PlayerController) return;

	int32 ScreenX = 0;
	int32 ScreenY = 0;

	PlayerController->GetViewportSize(ScreenX, ScreenY);

	FVector2D ScreenCenter = FVector2D(ScreenX / 2, ScreenY / 2);

	FVector WorldLocation = FVector::ZeroVector;
	FVector WorldDirection = FVector::ForwardVector;

	PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

	const FVector TraceTarget = WorldLocation + (WorldDirection * TraceDistance);
	DrawDebugLine(GetWorld(), WorldLocation, TraceTarget, FColor::Red);
}
