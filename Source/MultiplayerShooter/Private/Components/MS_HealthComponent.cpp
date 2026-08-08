// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_HealthComponent.h"

#include "Net/UnrealNetwork.h"


UMS_HealthComponent::UMS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UMS_HealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMS_HealthComponent, CurrentHealth);
	DOREPLIFETIME(UMS_HealthComponent, MaxHealth);
}

void UMS_HealthComponent::InitializeHealth()
{
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::OnRep_MaxHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::RequestHealthChange(float Delta)
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		ApplyHealthChange(Delta);
	}
	else
	{
		ServerRequestHealthChange(Delta);
	}
}

void UMS_HealthComponent::ServerRequestHealthChange_Implementation(float Delta)
{
	ApplyHealthChange(Delta);
}

void UMS_HealthComponent::ApplyHealthChange(float Delta)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
}
