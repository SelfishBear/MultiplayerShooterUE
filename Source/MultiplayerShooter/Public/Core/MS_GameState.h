// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MS_GameState.generated.h"

class AMS_PlayerState;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAdded, APlayerState*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoved, APlayerState*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinnerChosen, APlayerState*, WinnerState);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AMS_GameState : public AGameState
{
	GENERATED_BODY()

public:
	AMS_GameState();

	void StartMatchTimer(float Duration);

	void SetWinnerState(AMS_PlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category="Match")
	float GetRemainingMatchTime() const;
	
	UPROPERTY(BlueprintAssignable, Category="Match")
	FOnWinnerChosen OnWinnerChosen;

	FOnPlayerStateAdded OnPlayerStateAdded;

	FOnPlayerStateRemoved OnPlayerStateRemoved;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UFUNCTION()
	void OnRep_WinnerPlayerState();

	UPROPERTY(ReplicatedUsing=OnRep_WinnerPlayerState)
	APlayerState* WinnerPlayerState;

	UPROPERTY(Replicated)
	double MatchEndServerTime = 0.0;
};
