// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MS_CapturePlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressChanged, float, NewCurrentProgress);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConqueredBy, AMS_PlayerCharacter*, ConquerCharacter);

class UWidgetComponent;
class AMS_PlayerCharacter;
class USphereComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_CapturePlatform : public AActor
{
	GENERATED_BODY()

public:
	AMS_CapturePlatform();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress")
	float ProgressFillRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress")
	float ProgressFillStep = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress")
	float HoldingPointRate = 0.4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress")
	float ProgressDecreaseRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress")
	float ProgressDecreaseStep = 0.1f;

	UPROPERTY(BlueprintAssignable, Category="Progress")
	FOnProgressChanged OnProgressChanged;

	UPROPERTY(BlueprintAssignable, Category="Progress")
	FOnConqueredBy OnConqueredBy;

	UFUNCTION(BlueprintCallable, Category="Progress")
	FORCEINLINE float GetMaxProgress() const { return MaxProgress; }

	UFUNCTION(BlueprintCallable, Category="Progress")
	FORCEINLINE float GetCurrentProgress() const { return CurrentProgress; }

	UFUNCTION(BlueprintCallable, Category="Progress")
	FORCEINLINE AMS_PlayerCharacter* GetCurrentConquerPlayer() const { return CurrentConquerPlayer; }

	UFUNCTION(BlueprintCallable, Category="Progress")
	FORCEINLINE bool IsFull() const { return CurrentProgress == MaxProgress; }

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAreaStopOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex);

private:
	void StartFillProgress();
	void StopFillProgress();
	void DecreaseFillProgress();

	bool CheckProgressCondition();

	void GiveCapturedPointReward();
	void GiveHoldingPointReward();

	void OnTickProgressTimer();
	void OnTickDecreaseProgressTimer();

	void OnPointHeld();

	UFUNCTION()
	void OnRep_ConquerPlayer();

	UFUNCTION()
	void OnRep_CurrentProgress();

	UPROPERTY()
	TArray<AMS_PlayerCharacter*> PlayersInArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_CurrentProgress)
	float CurrentProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progress", meta=(AllowPrivateAccess=true))
	float MaxProgress;

	UPROPERTY(ReplicatedUsing=OnRep_ConquerPlayer)
	AMS_PlayerCharacter* CurrentConquerPlayer;

	FTimerHandle HoldingPointTimer;
	FTimerHandle StartProgressTimer;
	FTimerHandle DecreaseProgressTimer;
};
