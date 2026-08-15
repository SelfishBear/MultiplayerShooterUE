// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MS_MainGameMode.generated.h"

class UMS_HealthComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_MainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMS_MainGameMode();

	UFUNCTION(BlueprintCallable, Category="Character")
	void RespawnCharacter(AController* Controller, UMS_HealthComponent* PlayerHealth);
};
