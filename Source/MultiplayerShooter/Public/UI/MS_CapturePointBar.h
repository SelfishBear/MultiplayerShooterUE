// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/MS_PlayerCharacter.h"
#include "MS_CapturePointBar.generated.h"


class AMS_CapturePlatform;
class UTextBlock;
class UProgressBar;

UCLASS()
class MULTIPLAYERSHOOTER_API UMS_CapturePointBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ConquerPlayerName;

	void Init(AMS_CapturePlatform* NewOwningPlatform);
	
private:
	UFUNCTION()
	void UpdateProgress(float NewProgress);
	
	UFUNCTION()
	void OnConqueredBy(AMS_PlayerCharacter* ConquerCharacter);
	
	TWeakObjectPtr<AMS_CapturePlatform> OwnerPlatform;
	
};
