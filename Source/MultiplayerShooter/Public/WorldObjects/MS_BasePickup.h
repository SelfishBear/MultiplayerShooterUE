// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MS_Pickable.h"
#include "MS_BasePickup.generated.h"


USTRUCT(Blueprintable)
struct FPickupAnimationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float Amplitude = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float Frequency = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float RotationSpeed = 20.0f;
};

class UBoxComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_BasePickup : public AActor, public IMS_Pickable
{
	GENERATED_BODY()

public:
	AMS_BasePickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FPickupAnimationSettings AnimationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	float RestoreDelay = 10.0f;
	
	UFUNCTION(BlueprintImplementableEvent, Category="VFX|Sound")
	void PlayPickUpCosmetics();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void Pickup(ACharacter* PickingCharacter) override;

	virtual void OnPickUp();

	virtual void OnRestored();

private:
	void Animate(float DeltaTime);

	void AnimateRotation(float DeltaTime);
	void AnimateLocation();

	FTimerHandle RestoreTimerHandle;

	FVector InitialLocation;
};
