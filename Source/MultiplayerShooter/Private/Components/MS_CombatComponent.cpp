// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_CombatComponent.h"

#include "Core/MS_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UMS_CombatComponent::UMS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UMS_CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMS_CombatComponent, bIsAiming);
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