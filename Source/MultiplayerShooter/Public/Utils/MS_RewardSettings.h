// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MS_RewardSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="RewardSettings"))
class MULTIPLAYERSHOOTER_API UMS_RewardSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Reward")
	int32 KillScore = 200;

	UPROPERTY(Config, EditAnywhere, Category="Reward")
	int32 CapturePointScore = 500;

	UPROPERTY(Config, EditAnywhere, Category="Reward")
	int32 HoldingPointScore = 5;
};
