// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_MainGameMode.h"

#include "Components/MS_HealthComponent.h"
#include "Core/MS_GameState.h"
#include "Core/MS_PlayerCharacter.h"
#include "Core/MS_PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

AMS_MainGameMode::AMS_MainGameMode()
{
	bUseSeamlessTravel = true;
}

void AMS_MainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (AMS_GameState* MS_GameState = GetGameState<AMS_GameState>())
	{
		MS_GameState->StartMatchTimer(MatchDuration);
	}

	GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, this, &AMS_MainGameMode::HandleMatchOver, MatchDuration,
	                                       false);
}

void AMS_MainGameMode::RespawnCharacter(AController* Controller, UMS_HealthComponent* PlayerHealth)
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);

	int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);

	AActor* SpawnPoint = SpawnPoints[RandomIndex];

	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;

	if (!IsValid(Pawn) || !IsValid(SpawnPoint)) return;

	Pawn->SetActorLocationAndRotation(
		SpawnPoint->GetActorLocation(),
		SpawnPoint->GetActorRotation(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics);

	PlayerHealth->OnRespawned.Broadcast();
}

void AMS_MainGameMode::HandleMatchOver()
{
	AMS_PlayerState* WinnerState = nullptr;
	for (auto BasePlayerState : GameState->PlayerArray)
	{
		if (AMS_PlayerState* MS_PlayerState = Cast<AMS_PlayerState>(BasePlayerState))
		{
			WinnerState = MS_PlayerState;
			if (MS_PlayerState->GetScore() > WinnerState->GetScore())
			{
				WinnerState = MS_PlayerState;
			}
			if (AMS_PlayerCharacter* PlayerCharacter = Cast<AMS_PlayerCharacter>(BasePlayerState->GetPawn()))
			{
				PlayerCharacter->ClientRemoveInput();
			}
		}
	}

	AMS_GameState* MS_GameState = GetGameState<AMS_GameState>();
	MS_GameState->SetWinnerState(WinnerState);

	GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, this, &AMS_MainGameMode::Travel, TimeBeforeLobbyQuit,
	                                       false, TimeBeforeLobbyQuit);


	UE_LOG(LogTemp, Warning, TEXT("AMS_MainGameMode::HandleMatchOver"));
}

void AMS_MainGameMode::Travel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/MultiplayerShooter/Map/L_Lobby?listen");
	}
}
