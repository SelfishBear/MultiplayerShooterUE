// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_LobbyGameMode.h"

#include "Core/MS_PlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"

AMS_LobbyGameMode::AMS_LobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerCharacterClass(
		TEXT("/Game/MultiplayerShooter/Blueprint/Core/BP_MS_PlayerCharacter"));

	if (PlayerCharacterClass.Succeeded())
	{
		DefaultPawnClass = PlayerCharacterClass.Class;
	}

	bUseSeamlessTravel = true;
}

void AMS_LobbyGameMode::StartGame()
{
	int32 ReadyPlayers = 0;
	int32 AllConnectedPlayers = 0;

	for (auto BasePlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		AMS_PlayerState* MS_PlayerState = Cast<AMS_PlayerState>(BasePlayerState);
		if (!MS_PlayerState) continue;

		AllConnectedPlayers++;

		if (MS_PlayerState->GetIsReady())
		{
			ReadyPlayers++;
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(
			                                 TEXT("All connected players: %d, Ready of them: %d"),
			                                 AllConnectedPlayers, ReadyPlayers));
	}

	if (AllConnectedPlayers == 0 || AllConnectedPlayers != ReadyPlayers) return;

	Travel();
}

void AMS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMS_LobbyGameMode::Travel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/MultiplayerShooter/Map/L_MainGameEgypt?listen");
	}
}
