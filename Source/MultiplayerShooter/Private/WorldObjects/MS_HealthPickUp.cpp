// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/MS_HealthPickUp.h"

#include "Components/MS_HealthComponent.h"
#include "Core/MS_PlayerCharacter.h"

AMS_HealthPickUp::AMS_HealthPickUp()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMS_HealthPickUp::Pickup(ACharacter* PickingCharacter)
{
	Super::Pickup(PickingCharacter);

	if (AMS_PlayerCharacter* MS_PlayerCharacter = Cast<AMS_PlayerCharacter>(PickingCharacter))
	{
		MS_PlayerCharacter->GetHealthComponent()->RequestHeal(HealAmount);
	}
	OnPickUp();
}
