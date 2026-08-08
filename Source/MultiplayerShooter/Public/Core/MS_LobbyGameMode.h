// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MS_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AMS_LobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMS_LobbyGameMode();
	
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
