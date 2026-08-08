// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_BaseHUD.h"

#include "Blueprint/UserWidget.h"

AMS_BaseHUD::AMS_BaseHUD()
{
}

void AMS_BaseHUD::BeginPlay()
{
	Super::BeginPlay();

	InitHUDWidget();
}

void AMS_BaseHUD::InitHUDWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!IsValid(PlayerController)) return;

	PlayerBaseHUDWidget = CreateWidget(PlayerController, PlayerBaseHUDWidgetClass);

	PlayerBaseHUDWidget->AddToViewport();
}
