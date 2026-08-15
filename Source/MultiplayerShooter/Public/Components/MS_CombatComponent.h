// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MS_CombatComponent.generated.h"

USTRUCT(Blueprintable)
struct FDamageHitEvent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float Damage = 10.0f;

	TWeakObjectPtr<AActor> DamageCauser;
};

class UMS_ShootSoundDataAsset;
class UMS_SurfaceVFXDataAsset;
class UNiagaraSystem;

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMS_CombatComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	FDamageHitEvent DamageEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float TraceDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float ShootCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	TObjectPtr<UMS_SurfaceVFXDataAsset> HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
	TObjectPtr<UMS_ShootSoundDataAsset> ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	TObjectPtr<UAnimMontage> FireMontage;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void RequestFire();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void SetAiming(bool NewIsAiming);

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	void SetCanFire(bool bNewFire) { bCanFire = bNewFire; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool CanFire() const { return bCanFire; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryFire();

	UFUNCTION(Category="Cosmetics", NetMulticast, Reliable)
	void PlayFireCosmetics();

	UFUNCTION(Category="Cosmetics", NetMulticast, Reliable)
	void PlayShootSound();

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool bNewIsAiming);

	UFUNCTION()
	void OnRep_IsAiming();

	void ApplyAimingState();

	UFUNCTION(Server, Reliable)
	void ServerFire(FVector_NetQuantize TraceTarget);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayHitVFX(const FHitResult& HitResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_IsAiming)
	bool bIsAiming = false;

	bool bCanFire = true;

	FTimerHandle FireTimerHandle;

	void PerformServerFire(const FVector& TraceTarget);

	void HandleFireTimer();

	void DebugTrace();

	void HandleFireState();
};
