// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_PlayerState.h"

#include "Net/UnrealNetwork.h"

void AMS_PlayerState::RequestDataChange()
{
	if (HasAuthority())
	{
		ChangeData();
	}
	else
	{
		ChangeDataServer();
	}
}

void AMS_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMS_PlayerState, PlayerScore);
	DOREPLIFETIME(AMS_PlayerState, Kills);
	DOREPLIFETIME(AMS_PlayerState, Deaths);
	DOREPLIFETIME(AMS_PlayerState, bIsReady);
}

void AMS_PlayerState::OnRep_PlayerScore()
{
	OnPlayerStatsChanged.Broadcast();
}

void AMS_PlayerState::OnRep_Kills()
{
	OnPlayerStatsChanged.Broadcast();
}

void AMS_PlayerState::OnRep_Deaths()
{
	OnPlayerStatsChanged.Broadcast();
}

void AMS_PlayerState::OnRep_IsReady()
{
	OnReadyStatusChanged.Broadcast(GetIsReady());

	UE_LOG(LogTemp, Warning, TEXT("OnRep_IsReady, %d"), bIsReady);
}

void AMS_PlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
	OnPlayerStatsChanged.Broadcast();
}

void AMS_PlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AMS_PlayerState* NewMSPlayerState = Cast<AMS_PlayerState>(PlayerState);
	if (!NewMSPlayerState) return;

	NewMSPlayerState->bIsReady = false;
}

void AMS_PlayerState::ChangeDataServer_Implementation()
{
	ChangeData();
}

void AMS_PlayerState::ChangeData()
{
	PlayerScore = FMath::RandRange(0, 32);
	Kills = FMath::RandRange(0, 100);
	Deaths = FMath::RandRange(0, 100);
	OnPlayerStatsChanged.Broadcast();
}

void AMS_PlayerState::RequestReadyState()
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		ChangeReadyState();
	}
	else
	{
		ServerChangeReadyState();
	}
}

void AMS_PlayerState::ServerChangeReadyState_Implementation()
{
	ChangeReadyState();
}

void AMS_PlayerState::ChangeReadyState()
{
	bIsReady = !bIsReady;
	OnReadyStatusChanged.Broadcast(GetIsReady());
}
