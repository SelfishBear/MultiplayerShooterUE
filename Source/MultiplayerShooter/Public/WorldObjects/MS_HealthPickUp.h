// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MS_BasePickup.h"
#include "MS_HealthPickUp.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_HealthPickUp : public AMS_BasePickup
{
	GENERATED_BODY()

public:
	AMS_HealthPickUp();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Heal")
	float HealAmount = 50.0f;

protected:
	virtual void Pickup(ACharacter* PickingCharacter) override;
};
