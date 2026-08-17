// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_CombatComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/MS_HealthComponent.h"
#include "Core/MS_PlayerCharacter.h"
#include "DataAssets/MS_ShootSoundDataAsset.h"
#include "DataAssets/MS_SurfaceVFXDataAsset.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
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

void UMS_CombatComponent::HandleFireState()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		return;
	}
	SetCanFire(false);
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UMS_CombatComponent::HandleFireTimer, ShootCooldown,
	                                       false);
}

bool UMS_CombatComponent::TryFire()
{
	HandleFireState();
	RequestFire();
	return true;
}

void UMS_CombatComponent::PlayFireCosmetics_Implementation()
{
	AMS_PlayerCharacter* PlayerCharacter = Cast<AMS_PlayerCharacter>(GetOwner());
	if (!PlayerCharacter) return;

	USkeletalMeshComponent* Mesh = PlayerCharacter->GetMesh();

	if (!Mesh) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(FireMontage);
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

void UMS_CombatComponent::PlayTracer_Implementation(FVector_NetQuantize Start, FVector_NetQuantize End)
{
	if (!TracerEffect) return;

	UNiagaraComponent* TracerComponent =
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TracerEffect,
			Start
		);

	if (!TracerComponent) return;

	TracerComponent->SetNiagaraVariableVec3(
		TEXT("User.BeamStart"),
		Start
	);

	TracerComponent->SetNiagaraVariableVec3(
		TEXT("User.BeamEnd"),
		End
	);
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
	USkeletalMeshComponent* CharacterMesh = Shooter->GetMesh();

	const FVector SocketLocation = CharacterMesh->GetSocketLocation(FName("Muzzle_01"));

	PlayFireCosmetics();
	PlayShootSound();

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
	CollisionParams.bReturnPhysicalMaterial = true;

	FHitResult HitResult;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
	                                                       ECollisionChannel::ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red);

	const FVector TracerEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
	PlayTracer(SocketLocation, TracerEnd);

	if (!bHit) return;
	
	PlayHitVFX(HitResult);

	AMS_PlayerCharacter* HitCharacter = Cast<AMS_PlayerCharacter>(HitResult.GetActor());
	if (!HitCharacter) return;

	if (UMS_HealthComponent* HealthComponent = HitCharacter->GetHealthComponent())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Hit Health Comp");
		}

		FDamageHitEvent DamageHitEvent = DamageEvent;
		DamageHitEvent.DamageCauser = Shooter;
		HealthComponent->RequestTakeDamage(DamageHitEvent);
	}
}

void UMS_CombatComponent::HandleFireTimer()
{
	SetCanFire(true);
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

void UMS_CombatComponent::PlayHitVFX_Implementation(const FHitResult& HitResult)
{
	UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();

	if (!PhysMaterial) return;

	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMaterial);

	if (!HitEffect->SurfaceEffects.Contains(SurfaceType)) return;

	FParticleData ParticleData = HitEffect->SurfaceEffects[SurfaceType];

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleData.VFX, HitResult.ImpactPoint,
	                                         HitResult.ImpactNormal.Rotation(), ParticleData.InitialEffectScale);
}

void UMS_CombatComponent::PlayShootSound_Implementation()
{
	if (!ShootSound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound->ShootSoundCue);
}
