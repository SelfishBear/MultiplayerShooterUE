// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_AmmoComponent.h"

#include "SkeletonTreeBuilder.h"
#include "Core/MS_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UMS_AmmoComponent::UMS_AmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UMS_AmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMS_AmmoComponent, CurrentAmmo);
	DOREPLIFETIME(UMS_AmmoComponent, ReserveAmmo);
	DOREPLIFETIME(UMS_AmmoComponent, bIsReloading);
}

void UMS_AmmoComponent::OnRep_CurrentAmmo()
{
	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::OnRep_ReserveAmmo()
{
	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::RequestTryReload()
{
	if (!GetOwner()) return;
	if (!CanReload()) return;

	if (GetOwner()->HasAuthority())
	{
		Reload();
	}
	else
	{
		ServerTryReload();
	}
}

void UMS_AmmoComponent::ServerTryReload_Implementation()
{
	Reload();
}

void UMS_AmmoComponent::Reload()
{
	const int32 MissingAmmo = MaxAmmo - CurrentAmmo;
	const int32 AmmoToLoad = FMath::Min(MissingAmmo, ReserveAmmo);

	CurrentAmmo += AmmoToLoad;
	ReserveAmmo -= AmmoToLoad;
	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::NetPlayReloadMontage_Implementation()
{
	if (!GetOwner()) return;

	if (AMS_PlayerCharacter* OwnerCharacter = Cast<AMS_PlayerCharacter>(GetOwner()))
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		AnimInstance->Montage_Play(ReloadAnimMontage);
	}
}

void UMS_AmmoComponent::RequestSpendAmmo()
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		SpendAmmo();
	}
	else
	{
		ServerSpendAmmo();
	}
}

void UMS_AmmoComponent::ServerSpendAmmo_Implementation()
{
	SpendAmmo();
}

void UMS_AmmoComponent::SpendAmmo()
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - 1, 0, MaxAmmo);
	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::RequestPlayReloadMontage()
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		NetPlayReloadMontage();
	}
	else
	{
		ServerPlayReloadMontage();
	}
}

void UMS_AmmoComponent::ServerPlayReloadMontage_Implementation()
{
	NetPlayReloadMontage();
}

void UMS_AmmoComponent::RequestAddReserveAmmo(int32 AmmoAmount)
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		AddReserveAmmo(AmmoAmount);
	}
	else
	{
		ServerAddReserveAmmo(AmmoAmount);
	}
}

void UMS_AmmoComponent::AddReserveAmmo(int32 AmmoAmount)
{
	ReserveAmmo = FMath::Clamp(ReserveAmmo + AmmoAmount, 0, MaxReserveAmmo);
	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetReserveAmmo());
}

void UMS_AmmoComponent::ServerAddReserveAmmo_Implementation(int32 AmmoAmount)
{
	AddReserveAmmo(AmmoAmount);
}
