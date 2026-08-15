// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MS_LobbyHUD.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_LobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMS_LobbyHUD();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> PlayerBaseHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	UUserWidget* PlayerBaseHUDWidget;

protected:
	virtual void BeginPlay() override;

private:
	void InitHUDWidget();
};
