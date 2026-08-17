// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerShooter/Public/Core/MS_PlayerCharacter.h"

#include "EnhancedInputComponent.h"

#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "Components/MS_AmmoComponent.h"

#include "Components/MS_CombatComponent.h"

#include "Components/MS_HealthComponent.h"

#include "Core/MS_MainGameMode.h"

#include "Core/MS_PlayerState.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/RootMotionSource.h"

#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/MS_Pickable.h"

#include "Net/UnrealNetwork.h"

#include "Subsystems/MS_RewardSubsystem.h"

#include "Utils/MS_RewardSettings.h"

class AMS_MainGameMode;

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

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMS_PlayerCharacter::OnCharacterCapsuleOverlap);

	HealthComponent = CreateDefaultSubobject<UMS_HealthComponent>("HealthComponent");
	CombatComponent = CreateDefaultSubobject<UMS_CombatComponent>("CombatComponent");
	AmmoComponent = CreateDefaultSubobject<UMS_AmmoComponent>("AmmoComponent");
}

void AMS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindMappingContext();
	InitializeHealth();
}

void AMS_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AMS_PlayerCharacter::UnPossessed()
{
	UnbindMappingContext();
	Super::UnPossessed();
}

void AMS_PlayerCharacter::OnCharacterCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!IsValid(OtherActor)) return;
	if (OtherActor->Implements<UMS_Pickable>())
	{
		if (IMS_Pickable* Pickable = Cast<IMS_Pickable>(OtherActor))
		{
			Pickable->Pickup(this);
		}
	}
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
		EnhancedInputComponent->BindAction(ReloadInputAction, ETriggerEvent::Started, this,
		                                   &AMS_PlayerCharacter::Reload);
		EnhancedInputComponent->BindAction(MouseWheelSensitivityAction, ETriggerEvent::Triggered, this,
		                                   &AMS_PlayerCharacter::ChangeMouseSensitivity);
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
	if (!HasAuthority()) return;
	HealthComponent->InitializeHealth();
	HealthComponent->OnDeath.AddDynamic(this, &AMS_PlayerCharacter::HandleCharacterDeath);
	HealthComponent->OnRespawned.AddDynamic(this, &AMS_PlayerCharacter::HandleRespawnCharacter);
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

	AddControllerYawInput(LookVector.X * MouseSensitivity);
	AddControllerPitchInput(LookVector.Y * MouseSensitivity);
}

void AMS_PlayerCharacter::StartAim()
{
	if (!CombatComponent) return;
	if (AmmoComponent->IsReloading()) return;

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
	if (!CombatComponent->IsAiming()) return;
	if (!AmmoComponent->IsEnoughAmmo()) return;
	if (AmmoComponent->IsReloading()) return;
	if (!CombatComponent->CanFire()) return;

	AmmoComponent->RequestSpendAmmo();
	CombatComponent->TryFire();
}

void AMS_PlayerCharacter::Reload()
{
	if (!AmmoComponent) return;
	if (!AmmoComponent->CanReload()) return;
	if (AmmoComponent->IsReloading()) return;

	AmmoComponent->RequestPlayReloadMontage();
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

void AMS_PlayerCharacter::HandleCharacterDeath(AActor* DamageCauser)
{
	HandleDeathEffect();
	HandleReward(DamageCauser);

	if (AMS_MainGameMode* MainGameMode = GetWorld()->GetAuthGameMode<AMS_MainGameMode>())
	{
		MainGameMode->RespawnCharacter(GetController(), GetHealthComponent());
	}
}

void AMS_PlayerCharacter::HandleRespawnCharacter()
{
	HealthComponent->RequestResetHeath();
}

void AMS_PlayerCharacter::HandleReward(AActor* RewardTo)
{
	const UMS_RewardSettings* RewardSettings = GetDefault<UMS_RewardSettings>();
	if (!RewardSettings) return;

	AMS_PlayerState* InstigatorPlayerState = GetPlayerState<AMS_PlayerState>();
	if (!InstigatorPlayerState) return;

	if (AMS_PlayerCharacter* PlayerCharacter = Cast<AMS_PlayerCharacter>(RewardTo))
	{
		if (AMS_PlayerState* RewardToPlayerState = PlayerCharacter->GetPlayerState<AMS_PlayerState>())
		{
			UMS_RewardSubsystem* RewardSubsystem = GetWorld()->GetSubsystem<UMS_RewardSubsystem>();
			if (!RewardSubsystem) return;

			RewardSubsystem->AddScore(RewardToPlayerState, RewardSettings->KillScore);
			RewardSubsystem->AddKills(RewardToPlayerState, 1);
			RewardSubsystem->AddDeath(InstigatorPlayerState, 1);
		}
	}
}

void AMS_PlayerCharacter::HandleDeathEffect_Implementation()
{
	PlayDeathEffects();
}

void AMS_PlayerCharacter::ChangeMouseSensitivity(const FInputActionValue& Value)
{
	const float WheelDelta = Value.Get<float>();

	MouseSensitivity = FMath::Clamp(MouseSensitivity + WheelDelta * MouseSensitivityStep, MinMouseSensitivity,
	                                MaxMouseSensitivity);
}
