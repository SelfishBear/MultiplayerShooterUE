// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_LobbyGameMode.h"

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

void AMS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (GameState->PlayerArray.Num() >= 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/MultiplayerShooter/Map/L_MainGameEgypt?listen");
		}
	}
}
