// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/MS_AmmoPickup.h"

#include "Components/MS_AmmoComponent.h"
#include "Core/MS_PlayerCharacter.h"


class AMS_PlayerCharacter;

AMS_AmmoPickup::AMS_AmmoPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMS_AmmoPickup::Pickup(ACharacter* PickingCharacter)
{
	Super::Pickup(PickingCharacter);

	if (AMS_PlayerCharacter* MS_PlayerCharacter = Cast<AMS_PlayerCharacter>(PickingCharacter))
	{
		MS_PlayerCharacter->GetAmmoComponent()->RequestAddReserveAmmo(AmmoAmount);
	}
	OnPickUp();
}
