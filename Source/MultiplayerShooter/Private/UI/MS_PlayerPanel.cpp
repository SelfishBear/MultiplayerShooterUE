// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MS_PlayerPanel.h"

#include "Components/TextBlock.h"
#include "Core/MS_PlayerState.h"
#include "GameFramework/GameStateBase.h"


void UMS_PlayerPanel::InitializePanel(APlayerState* BasePlayerState)
{
	if (!BasePlayerState) return;

	OwnedPlayerState = Cast<AMS_PlayerState>(BasePlayerState);
	OwnedPlayerState->OnPlayerStatsChanged.AddUObject(this, &UMS_PlayerPanel::RefreshPanelData);
	RefreshPanelData();
}

void UMS_PlayerPanel::NativeDestruct()
{
	if (!OwnedPlayerState.IsValid()) return;
	OwnedPlayerState->OnPlayerStatsChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void UMS_PlayerPanel::RefreshPanelData()
{
	if (OwnedPlayerState.IsValid())
	{
		FString PlayerName = OwnedPlayerState->GetPlayerName();
		float PlayerScore = OwnedPlayerState->GetPlayerScore();
		int32 Deaths = OwnedPlayerState->GetDeaths();
		int32 Kills = OwnedPlayerState->GetKills();

		SetDataText(PlayerName, Kills, Deaths, PlayerScore);
	}
}

void UMS_PlayerPanel::SetDataText(const FString& PlayerName, const float Kills, const float Deaths, const float Score)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
	PlayerKillsText->SetText(FText::AsNumber(Kills));
	PlayerScoreText->SetText(FText::AsNumber(Score));
	PlayerDeathText->SetText(FText::AsNumber(Deaths));
}
