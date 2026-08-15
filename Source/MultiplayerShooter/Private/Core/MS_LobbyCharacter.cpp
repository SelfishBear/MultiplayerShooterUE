// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_LobbyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

AMS_LobbyCharacter::AMS_LobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMS_LobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindMappingContext();
}

void AMS_LobbyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AMS_LobbyCharacter::UnPossessed()
{
	UnbindMappingContext();
	Super::UnPossessed();
}


void AMS_LobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMS_LobbyCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMS_LobbyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AMS_LobbyCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this,
		                                   &AMS_LobbyCharacter::StopJumping);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to setup input component."));
		}
	}
}

void AMS_LobbyCharacter::BindMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!EnhancedInput) return;

	EnhancedInput->AddMappingContext(InputMappingContext, 0);
}

void AMS_LobbyCharacter::UnbindMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!EnhancedInput) return;

	EnhancedInput->RemoveMappingContext(InputMappingContext);
}

void AMS_LobbyCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementVector.IsZero()) return;
	if (!Controller) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMS_LobbyCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (LookVector.IsZero()) return;
	if (!Controller) return;

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
