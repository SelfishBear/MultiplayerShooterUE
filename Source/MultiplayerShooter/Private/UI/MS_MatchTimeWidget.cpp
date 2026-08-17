// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MS_MatchTimeWidget.h"

#include "Components/TextBlock.h"
#include "Core/MS_GameState.h"

void UMS_MatchTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ApplyMatchTime();
	GetWorld()->GetTimerManager().SetTimer(MatchTimerUpdateHandle, this, &UMS_MatchTimeWidget::ApplyMatchTime, 0.1f,
	                                       true);
}

void UMS_MatchTimeWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MatchTimerUpdateHandle);
	}
	Super::NativeDestruct();
}

void UMS_MatchTimeWidget::ApplyMatchTime()
{
	if (!MatchTimeText) return;

	const AMS_GameState* GameState = GetWorld()->GetGameState<AMS_GameState>();
	if (!GameState) return;

	const int32 SecondsLeft = FMath::CeilToInt(
		GameState->GetRemainingMatchTime()
	);

	if (SecondsLeft == LastShownSeconds) return;

	LastShownSeconds = SecondsLeft;

	const int32 Minutes = SecondsLeft / 60;
	const int32 Seconds = SecondsLeft % 60;

	MatchTimeText->SetText(FText::FromString(
		FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)
	));
}
