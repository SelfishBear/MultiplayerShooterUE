// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MS_GameInfoWidget.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMS_GameInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	TSubclassOf<UUserWidget> PanelWidgetClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> PanelContainer;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void RefreshBoard();
	void HandlePlayerAdded(APlayerState* PlayerState);
	void HandlePlayerRemoved(APlayerState* PlayerState);
};
