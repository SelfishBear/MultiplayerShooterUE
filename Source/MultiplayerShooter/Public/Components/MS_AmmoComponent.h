// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MS_AmmoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewCurrentAmmo, int32, NewReserveAmmo);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMS_AmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMS_AmmoComponent();

	UFUNCTION(BlueprintCallable, Category="Animation")
	void RequestPlayReloadMontage();

	UFUNCTION(BlueprintCallable, Category="Ammo")
	void RequestTryReload();

	UFUNCTION(BlueprintCallable, Category="Ammo")
	void RequestAddReserveAmmo(int32 AmmoAmount);

	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE bool CanReload() const { return CurrentAmmo < MaxAmmo && ReserveAmmo > 0; }


	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE bool IsEnoughAmmo() const { return CurrentAmmo > 0; }


	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }


	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE int32 GetReserveAmmo() const { return ReserveAmmo; }


	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE void SetReloading(bool bNewReloading) { bIsReloading = bNewReloading; }


	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE bool IsReloading() const { return bIsReloading; }


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UAnimMontage> ReloadAnimMontage;

	UPROPERTY(BlueprintAssignable, Category="Ammo")
	FOnAmmoChanged OnAmmoChanged;

	void RequestSpendAmmo();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentAmmo();

	UFUNCTION()
	void OnRep_ReserveAmmo();

	UFUNCTION(Server, Reliable)
	void ServerTryReload();

	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerSpendAmmo();

	void SpendAmmo();

	UFUNCTION(Server, Unreliable)
	void ServerPlayReloadMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void NetPlayReloadMontage();

	void AddReserveAmmo(int32 AmmoAmount);

	UFUNCTION(Server, Reliable)
	void ServerAddReserveAmmo(int32 AmmoAmount);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo", Replicated,
		meta=(AllowPrivateAccess = true))
	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo", ReplicatedUsing=OnRep_CurrentAmmo,
		meta=(ClampMin = 0, ClampMax = 100, AllowPrivateAccess = true))
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo",
		meta=(ClampMin = 0, ClampMax = 100, AllowPrivateAccess = true))
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo", ReplicatedUsing=OnRep_ReserveAmmo,
		meta=(ClampMin = 0, ClampMax = 100, AllowPrivateAccess = true))
	int32 ReserveAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo",
		meta=(ClampMin = 0, ClampMax = 100, AllowPrivateAccess = true))
	int32 MaxReserveAmmo;
};
