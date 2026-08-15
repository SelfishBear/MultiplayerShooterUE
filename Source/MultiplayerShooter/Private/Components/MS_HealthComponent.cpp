// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MS_HealthComponent.h"

#include "Components/MS_CombatComponent.h"
#include "Core/MS_MainGameMode.h"
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
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
		OnHealthChanged.Broadcast(CurrentHealth);
	}
}

void UMS_HealthComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth);

	const AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Warning,
	       TEXT("[%s] Health replicated: %.1f | Role: %s"),
	       *GetNameSafe(Owner),
	       CurrentHealth,
	       Owner && Owner->HasAuthority() ? TEXT("Server") : TEXT("Client"))
}

void UMS_HealthComponent::OnRep_MaxHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::RequestTakeDamage(const FDamageHitEvent& DamageHitEvent)
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		TakeDamage(DamageHitEvent);
	}
	else
	{
		ServerTakeDamage(DamageHitEvent);
	}
}

void UMS_HealthComponent::TakeDamage(const FDamageHitEvent& DamageHitEvent)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageHitEvent.Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		Death(DamageHitEvent);
	}
	UE_LOG(LogTemp, Warning, TEXT("Took Damage: %f"), CurrentHealth);
}

void UMS_HealthComponent::ServerTakeDamage_Implementation(const FDamageHitEvent& DamageHitEvent)
{
	TakeDamage(DamageHitEvent);
}

void UMS_HealthComponent::RequestHeal(float HealAmount)
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		Heal(HealAmount);
	}
	else
	{
		ServerHeal(HealAmount);
	}
}

void UMS_HealthComponent::Heal(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::ServerHeal_Implementation(float HealAmount)
{
	Heal(HealAmount);
}

void UMS_HealthComponent::Death(const FDamageHitEvent& DamageHitEvent)
{
	if (DamageHitEvent.DamageCauser.IsValid())
	{
		OnDeath.Broadcast(DamageHitEvent.DamageCauser.Get());
	}
}

void UMS_HealthComponent::RequestResetHeath()
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		ResetHealth();
	}
	else
	{
		ServerResetHealth();
	}
}

void UMS_HealthComponent::ResetHealth()
{
	CurrentHealth = FMath::Clamp(CurrentHealth + MaxHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UMS_HealthComponent::ServerResetHealth_Implementation()
{
	ResetHealth();
}
