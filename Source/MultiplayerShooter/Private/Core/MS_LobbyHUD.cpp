// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_LobbyHUD.h"

#include "Blueprint/UserWidget.h"

AMS_LobbyHUD::AMS_LobbyHUD()
{
}

void AMS_LobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	InitHUDWidget();
}

void AMS_LobbyHUD::InitHUDWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!IsValid(PlayerController)) return;

	PlayerBaseHUDWidget = CreateWidget(PlayerController, PlayerBaseHUDWidgetClass);

	PlayerBaseHUDWidget->AddToViewport();
}
