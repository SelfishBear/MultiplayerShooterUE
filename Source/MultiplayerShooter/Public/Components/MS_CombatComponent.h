// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MS_CombatComponent.generated.h"


UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMS_CombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float TraceDistance = 10000.0f;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void RequestFire();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void SetAiming(bool NewIsAiming);

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsAiming() const { return bIsAiming; }

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool bNewIsAiming);

	UFUNCTION()
	void OnRep_IsAiming();

	void ApplyAimingState();

	UFUNCTION(Server, Reliable)
	void ServerFire(FVector_NetQuantize TraceTarget);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_IsAiming)
	bool bIsAiming = false;

	void PerformServerFire(const FVector& TraceTarget);
	
	void DebugTrace();
};
