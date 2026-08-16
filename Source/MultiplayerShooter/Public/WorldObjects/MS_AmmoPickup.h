// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MS_BasePickup.h"
#include "MS_AmmoPickup.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_AmmoPickup : public AMS_BasePickup
{
	GENERATED_BODY()

public:
	AMS_AmmoPickup();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	int32 AmmoAmount = 15;

protected:
	virtual void Pickup(ACharacter* PickingCharacter) override;
};
