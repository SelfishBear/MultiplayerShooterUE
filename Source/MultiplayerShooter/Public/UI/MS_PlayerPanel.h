// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MS_PlayerPanel.generated.h"

class AMS_PlayerState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMS_PlayerPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerKillsText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerDeathText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerScoreText;

	UFUNCTION(BlueprintCallable, Category="UI")
	void InitializePanel(APlayerState* BasePlayerState);

	TWeakObjectPtr<AMS_PlayerState> OwnedPlayerState;

protected:
	virtual void NativeDestruct() override;

private:
	void RefreshPanelData();
	void SetDataText(const FString& PlayerName, const float Kills, const float Deaths, const float Score);
};
