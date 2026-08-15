// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_LobbyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/MS_LobbyGameMode.h"
#include "Core/MS_PlayerState.h"

class UEnhancedInputLocalPlayerSubsystem;

AMS_LobbyPlayerController::AMS_LobbyPlayerController()
{
}

void AMS_LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	RegisterMappingContext();
}

void AMS_LobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AMS_LobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SetReadyAction, ETriggerEvent::Started, this,
		                                   &AMS_LobbyPlayerController::SetReady);

		EnhancedInputComponent->BindAction(HostStartGameAction, ETriggerEvent::Started, this,
		                                   &AMS_LobbyPlayerController::HostStartGame);
	}
}

void AMS_LobbyPlayerController::SetReady()
{
	AMS_PlayerState* BasePlayerState = GetPlayerState<AMS_PlayerState>();
	if (!BasePlayerState) return;

	BasePlayerState->RequestReadyState();
}

void AMS_LobbyPlayerController::HostStartGame()
{
	if (!HasAuthority()) return;

	AMS_LobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AMS_LobbyGameMode>();

	if (!GameMode) return;

	GameMode->StartGame();
}

void AMS_LobbyPlayerController::RegisterMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!EnhancedInputSubsystem) return;

	EnhancedInputSubsystem->AddMappingContext(LobbyControlMappingContext, 0);
}

void AMS_LobbyPlayerController::UnregisterMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!EnhancedInputSubsystem) return;

	EnhancedInputSubsystem->RemoveMappingContext(LobbyControlMappingContext);
}
