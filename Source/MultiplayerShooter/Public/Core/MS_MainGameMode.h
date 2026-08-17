// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MS_MainGameMode.generated.h"

class UMS_HealthComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_MainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMS_MainGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Match")
	float MatchDuration = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Match")
	float TimeBeforeLobbyQuit = 10.0f;

	UFUNCTION(BlueprintCallable, Category="Character")
	void RespawnCharacter(AController* Controller, UMS_HealthComponent* PlayerHealth);

protected:
	virtual void BeginPlay() override;

private:
	void HandleMatchOver();
	void Travel();

	FTimerHandle MatchTimerHandle;
	FTimerHandle LobbyTimerHandle;
};
