// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MS_RewardSubsystem.generated.h"

class UMS_RewardSettings;

UCLASS()
class MULTIPLAYERSHOOTER_API UMS_RewardSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Reward")
	void AddScore(APlayerState* PlayerState, int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Reward")
	void AddKills(APlayerState* PlayerState, int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Reward")
	void AddDeath(APlayerState* PlayerState, int32 Amount);
};
