// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MS_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMS_HealthComponent();

	void InitializeHealth();
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void RequestHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChangedSignature OnHealthChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_CurrentHealth();
	
	UFUNCTION()
	void OnRep_MaxHealth();

private:

	UFUNCTION(Server, Reliable)
	void ServerRequestHealthChange(float Delta);

	void ApplyHealthChange(float Delta);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin=0.0f, ClampMax = 100.0f),
		ReplicatedUsing = OnRep_MaxHealth, meta=(AllowPrivateAccess = true))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin=0.0f, ClampMax = 100.0f),
		ReplicatedUsing = OnRep_CurrentHealth, meta=(AllowPrivateAccess = true))
	float CurrentHealth = 100.0f;
};
