// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMS_PlayerController::AMS_PlayerController()
{
}

void AMS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	RegisterMappingContext();
}

void AMS_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AMS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ShowGameInfoAction, ETriggerEvent::Started, this,
		                                   &AMS_PlayerController::ShowGameInfo);
		EnhancedInputComponent->BindAction(ShowGameInfoAction, ETriggerEvent::Completed, this,
		                                   &AMS_PlayerController::HideGameInfo);
	}
}

void AMS_PlayerController::RegisterMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!EnhancedInputSubsystem) return;

	EnhancedInputSubsystem->AddMappingContext(UIMappingContext, 0);
}

void AMS_PlayerController::UnregisterMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!EnhancedInputSubsystem) return;
	EnhancedInputSubsystem->RemoveMappingContext(UIMappingContext);
}
