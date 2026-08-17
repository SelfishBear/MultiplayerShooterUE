// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MS_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AMS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMS_PlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> UIMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> ShowGameInfoAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> ChangeGraphicsAction;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameInfo();

	UFUNCTION(BlueprintImplementableEvent)
	void HideGameInfo();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeGraphics();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

private:
	void RegisterMappingContext();
	void UnregisterMappingContext();
	
	void ChangeGraphicsContext();
};
