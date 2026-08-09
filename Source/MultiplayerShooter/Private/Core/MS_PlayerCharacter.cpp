// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerShooter/Public/Core/MS_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/MS_CombatComponent.h"
#include "Components/MS_HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

AMS_PlayerCharacter::AMS_PlayerCharacter()
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

	HealthComponent = CreateDefaultSubobject<UMS_HealthComponent>("HealthComponent");
	CombatComponent = CreateDefaultSubobject<UMS_CombatComponent>("CombatComponent");
}

void AMS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindMappingContext();
	InitializeHealth();
}

void AMS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMS_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AMS_PlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
	UnbindMappingContext();
}

void AMS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMS_PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMS_PlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AMS_PlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this,
		                                   &AMS_PlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Started, this,
		                                   &AMS_PlayerCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Completed, this,
		                                   &AMS_PlayerCharacter::StopAim);
		EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &AMS_PlayerCharacter::Fire);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to setup input component."));
		}
	}
}

void AMS_PlayerCharacter::InitializeHealth()
{
	HealthComponent->InitializeHealth();
}

void AMS_PlayerCharacter::BindMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!EnhancedInput) return;

	EnhancedInput->AddMappingContext(InputMappingContext, 0);
}

void AMS_PlayerCharacter::UnbindMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!EnhancedInput) return;

	EnhancedInput->RemoveMappingContext(InputMappingContext);
}

void AMS_PlayerCharacter::Move(const FInputActionValue& Value)
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

void AMS_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (LookVector.IsZero()) return;
	if (!Controller) return;

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AMS_PlayerCharacter::StartAim()
{
	if (!CombatComponent) return;

	CombatComponent->SetAiming(true);
}

void AMS_PlayerCharacter::StopAim()
{
	if (!CombatComponent) return;

	CombatComponent->SetAiming(false);
}

void AMS_PlayerCharacter::Fire()
{
	if (!CombatComponent) return; 
	
	CombatComponent->RequestFire();
}

void AMS_PlayerCharacter::Reload()
{
}

void AMS_PlayerCharacter::ApplyAimingMovementSettings()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (!Movement) return;

	if (!CombatComponent) return;

	Movement->MaxWalkSpeed = CombatComponent->IsAiming()
		                         ? MovementSettings.AimMaxWalkSpeed
		                         : MovementSettings.DefaultMaxWalkSpeed;

	Movement->bOrientRotationToMovement = !CombatComponent->IsAiming();
}
