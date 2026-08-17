// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_GameState.h"

#include "Net/UnrealNetwork.h"

AMS_GameState::AMS_GameState()
{
}

void AMS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMS_GameState, MatchEndServerTime);
}

void AMS_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	OnPlayerStateAdded.Broadcast(PlayerState);
}

void AMS_GameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	OnPlayerStateRemoved.Broadcast(PlayerState);
}

void AMS_GameState::StartMatchTimer(float Duration)
{
	if (!HasAuthority()) return;

	MatchEndServerTime = GetServerWorldTimeSeconds() + Duration;
	ForceNetUpdate();
}

float AMS_GameState::GetRemainingMatchTime() const
{
	if (MatchEndServerTime <= 0.0)
	{
		return 0.0f;
	}

	return static_cast<float>(FMath::Max(0.0, MatchEndServerTime - GetServerWorldTimeSeconds()));
}
