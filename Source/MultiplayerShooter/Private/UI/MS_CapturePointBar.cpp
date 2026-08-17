// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MS_CapturePointBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/MS_PlayerState.h"
#include "WorldObjects/MS_CapturePlatform.h"

void UMS_CapturePointBar::Init(AMS_CapturePlatform* NewOwningPlatform)
{
	if (!NewOwningPlatform) return;

	OwnerPlatform = NewOwningPlatform;
	OwnerPlatform->OnProgressChanged.AddDynamic(this, &UMS_CapturePointBar::UpdateProgress);
	OwnerPlatform->OnConqueredBy.AddDynamic(this, &UMS_CapturePointBar::OnConqueredBy);
}

void UMS_CapturePointBar::UpdateProgress(float NewProgress)
{
	const float PercentProgress = NewProgress / OwnerPlatform->GetMaxProgress();
	ProgressBar->SetPercent(PercentProgress);
}

void UMS_CapturePointBar::OnConqueredBy(AMS_PlayerCharacter* ConquerCharacter)
{
	if (AMS_PlayerState* PlayerState = ConquerCharacter->GetPlayerState<AMS_PlayerState>())
	{
		ConquerPlayerName->SetText(FText::FromString(PlayerState->GetPlayerName()));
	}
	else
	{
		ConquerPlayerName->SetText(FText::FromString(""));
	}
}
