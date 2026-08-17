// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MS_MatchTimeWidget.generated.h"


class UTextBlock;

UCLASS()
class MULTIPLAYERSHOOTER_API UMS_MatchTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MatchTimeText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void ApplyMatchTime();

	FTimerHandle MatchTimerUpdateHandle;
	int32 LastShownSeconds = INDEX_NONE;
};
