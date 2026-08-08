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

	UFUNCTION(BlueprintCallable, Category="Combat")
	void SetAiming(bool NewIsAiming);

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsAiming() const { return bIsAiming; }
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool bNewIsAiming);
	
	UFUNCTION()
	void OnRep_IsAiming();
	
	void ApplyAimingState();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess=true), ReplicatedUsing=OnRep_IsAiming)
	bool bIsAiming = false;
};
