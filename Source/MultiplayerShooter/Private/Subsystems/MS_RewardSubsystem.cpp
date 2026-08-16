// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MS_RewardSubsystem.h"

#include "Core/MS_PlayerState.h"
#include "Utils/MS_RewardSettings.h"

void UMS_RewardSubsystem::AddScore(APlayerState* PlayerState, int32 Amount)
{
	if (AMS_PlayerState* MS_PlayerState = Cast<AMS_PlayerState>(PlayerState))
	{
		MS_PlayerState->ServerAddScore(Amount);
	}
}

void UMS_RewardSubsystem::AddKills(APlayerState* PlayerState, int32 Amount)
{
	if (AMS_PlayerState* MS_PlayerState = Cast<AMS_PlayerState>(PlayerState))
	{
		MS_PlayerState->ServerAddKills(Amount);
	}
}

void UMS_RewardSubsystem::AddDeath(APlayerState* PlayerState, int32 Amount)
{
	if (AMS_PlayerState* MS_PlayerState = Cast<AMS_PlayerState>(PlayerState))
	{
		MS_PlayerState->ServerAddDeath(Amount);
	}
}

