// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MS_GameInfoWidget.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Core/MS_GameState.h"
#include "GameFramework/GameState.h"
#include "UI/MS_PlayerPanel.h"

void UMS_GameInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AMS_GameState* GameState = Cast<AMS_GameState>(GetWorld()->GetGameState()))
	{
		GameState->OnPlayerStateAdded.AddUObject(this, &UMS_GameInfoWidget::HandlePlayerAdded);
		GameState->OnPlayerStateRemoved.AddUObject(this, &UMS_GameInfoWidget::HandlePlayerRemoved);
	}

	RefreshBoard();
}

void UMS_GameInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AMS_GameState* GameState = Cast<AMS_GameState>(GetWorld()->GetGameState()))
	{
		GameState->OnPlayerStateAdded.RemoveAll(this);
		GameState->OnPlayerStateRemoved.RemoveAll(this);
	}
}

void UMS_GameInfoWidget::HandlePlayerAdded(APlayerState* PlayerState)
{
	RefreshBoard();
}

void UMS_GameInfoWidget::HandlePlayerRemoved(APlayerState* PlayerState)
{
	RefreshBoard();
}

void UMS_GameInfoWidget::RefreshBoard()
{
	PanelContainer->ClearChildren();

	for (auto PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		UMS_PlayerPanel* PanelWidget = CreateWidget<UMS_PlayerPanel>(GetOwningPlayer(), PanelWidgetClass);

		if (PanelWidget)
		{
			PanelWidget->InitializePanel(PlayerState);
			if (UVerticalBoxSlot* BoxSlot = PanelContainer->AddChildToVerticalBox(PanelWidget))
			{
				const float BottomSpacing = 8.0f;
				BoxSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, BottomSpacing));
			}
		}
	}
}
