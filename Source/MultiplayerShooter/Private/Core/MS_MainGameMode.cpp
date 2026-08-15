// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MS_MainGameMode.h"

#include "Components/MS_HealthComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AMS_MainGameMode::AMS_MainGameMode()
{
}

void AMS_MainGameMode::RespawnCharacter(AController* Controller, UMS_HealthComponent* PlayerHealth)
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);

	int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);

	AActor* SpawnPoint = SpawnPoints[RandomIndex];

	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;

	if (!IsValid(Pawn) || !IsValid(SpawnPoint)) return;

	Pawn->SetActorLocationAndRotation(
		SpawnPoint->GetActorLocation(),
		SpawnPoint->GetActorRotation(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics);

	PlayerHealth->OnRespawned.Broadcast();
}
