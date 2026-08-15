// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MS_LobbyPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_LobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMS_LobbyPlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> LobbyControlMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> SetReadyAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> HostStartGameAction;

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

private:
	void SetReady();
	void HostStartGame();
	
	void RegisterMappingContext();
	void UnregisterMappingContext();
};
