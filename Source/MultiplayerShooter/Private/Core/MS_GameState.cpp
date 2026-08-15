// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_GameState.h"

AMS_GameState::AMS_GameState()
{
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
