// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MS_PlayerCharacter.generated.h"

class UMS_AmmoComponent;
class UMS_CombatComponent;
class UMS_HealthComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

USTRUCT(Blueprintable)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float DefaultMaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float AimMaxWalkSpeed = 300.0f;
};

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMS_PlayerCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UMS_CombatComponent> CombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UMS_AmmoComponent> AmmoComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UMS_HealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> AimInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> FireInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> ReloadInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> DashInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MouseWheelSensitivityAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(ClampMin = 0.01, ClampMax = 1.0))
	float MouseSensitivityStep = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	float MinMouseSensitivity = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	float MaxMouseSensitivity = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	FMovementSettings MovementSettings;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE UMS_HealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	FORCEINLINE UMS_CombatComponent* GetCombatComponent() const { return CombatComponent; }

	UFUNCTION(BlueprintCallable, Category="Ammo")
	FORCEINLINE UMS_AmmoComponent* GetAmmoComponent() const { return AmmoComponent; }

	UFUNCTION(NetMulticast, Unreliable)
	void HandleDeathEffect();

	UFUNCTION(BlueprintImplementableEvent, Category="VFX")
	void PlayDeathEffects();
	
	void ApplyAimingMovementSettings();
	
	UFUNCTION(Client, Reliable)
	void ClientRemoveInput();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void UnPossessed() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void OnCharacterCapsuleOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

private:
	void InitializeHealth();

	void BindMappingContext();
	void UnbindMappingContext();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartAim();
	void StopAim();

	void Fire();
	void Reload();
	
	void ChangeMouseSensitivity(const FInputActionValue& Value);

	UFUNCTION()
	void HandleCharacterDeath(AActor* DamageCauser);

	UFUNCTION()
	void HandleRespawnCharacter();

	void HandleReward(AActor* RewardTo);
};
