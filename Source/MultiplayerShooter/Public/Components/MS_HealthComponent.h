// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MS_HealthComponent.generated.h"

struct FDamageHitEvent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DamageCauser);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawned);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMS_HealthComponent();

	UFUNCTION(BlueprintCallable, Category="Health")
	void RequestTakeDamage(const FDamageHitEvent& DamageHitEvent);

	UFUNCTION(BlueprintCallable, Category="Health")
	void RequestHeal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void RequestResetHeath();

	UFUNCTION(BlueprintCallable, Category="Health")
	void Death(const FDamageHitEvent& DamageHitEvent);

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnRespawned OnRespawned;

	void InitializeHealth();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_MaxHealth();

	void TakeDamage(const FDamageHitEvent& DamageHitEvent);

	UFUNCTION(Server, Reliable)
	void ServerTakeDamage(const FDamageHitEvent& DamageHitEvent);

	void Heal(float HealAmount);

	UFUNCTION(Server, Reliable)
	void ServerHeal(float HealAmount);

	void ResetHealth();

	UFUNCTION(Server, Reliable)
	void ServerResetHealth();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin=0.0f, ClampMax = 100.0f),
		ReplicatedUsing = OnRep_MaxHealth, meta=(AllowPrivateAccess = true))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin=0.0f, ClampMax = 100.0f),
		ReplicatedUsing = OnRep_CurrentHealth, meta=(AllowPrivateAccess = true))
	float CurrentHealth = 100.0f;
};
