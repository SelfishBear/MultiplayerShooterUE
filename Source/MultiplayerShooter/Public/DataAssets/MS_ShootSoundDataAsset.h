// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MS_ShootSoundDataAsset.generated.h"


UCLASS()
class MULTIPLAYERSHOOTER_API UMS_ShootSoundDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
	TObjectPtr<USoundBase> ShootSoundCue;
};
