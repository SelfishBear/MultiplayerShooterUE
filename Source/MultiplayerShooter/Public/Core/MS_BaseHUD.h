// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MS_BaseHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseHUDWidgetInitialized, UUserWidget*, BaseHUDWidget);

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_BaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMS_BaseHUD();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> PlayerBaseHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	UUserWidget* PlayerBaseHUDWidget;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UI")
	FOnBaseHUDWidgetInitialized OnBaseHUDWidgetInitialized;

protected:
	virtual void BeginPlay() override;

private:
	void InitHUDWidget();
};
