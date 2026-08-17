// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/MS_CapturePlatform.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/MS_PlayerCharacter.h"
#include "Core/MS_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/MS_RewardSubsystem.h"
#include "UI/MS_CapturePointBar.h"
#include "Utils/MS_RewardSettings.h"

class UMS_RewardSubsystem;
class UMS_RewardSettings;

AMS_CapturePlatform::AMS_CapturePlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(StaticMesh);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(StaticMesh);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMS_CapturePlatform::OnAreaOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AMS_CapturePlatform::OnAreaStopOverlap);
}

void AMS_CapturePlatform::BeginPlay()
{
	Super::BeginPlay();

	if (UMS_CapturePointBar* CapturePointBar = Cast<UMS_CapturePointBar>(WidgetComponent->GetWidget()))
	{
		CapturePointBar->Init(this);
	}
}

void AMS_CapturePlatform::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMS_CapturePlatform, CurrentConquerPlayer);
	DOREPLIFETIME(AMS_CapturePlatform, CurrentProgress);
}

void AMS_CapturePlatform::OnAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor->IsA(AMS_PlayerCharacter::StaticClass())) return;
	AMS_PlayerCharacter* PlayerCharacter = Cast<AMS_PlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	PlayersInArea.AddUnique(PlayerCharacter);

	if (CheckProgressCondition())
	{
		CurrentConquerPlayer = PlayersInArea[0];
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		StartFillProgress();
	}
	else if (PlayersInArea.Num() > 1)
	{
		CurrentConquerPlayer = nullptr;
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		StopFillProgress();
	}
	else
	{
		CurrentConquerPlayer = nullptr;
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		DecreaseFillProgress();
	}
}

void AMS_CapturePlatform::OnAreaStopOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (!OtherActor->IsA(AMS_PlayerCharacter::StaticClass())) return;

	if (!OtherActor->IsA(AMS_PlayerCharacter::StaticClass())) return;
	AMS_PlayerCharacter* PlayerCharacter = Cast<AMS_PlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	PlayersInArea.RemoveSingle(PlayerCharacter);

	if (CheckProgressCondition())
	{
		CurrentConquerPlayer = PlayersInArea[0];
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		StartFillProgress();
	}
	else if (PlayersInArea.Num() > 1)
	{
		CurrentConquerPlayer = nullptr;
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		StopFillProgress();
	}
	else
	{
		CurrentConquerPlayer = nullptr;
		if (IsValid(CurrentConquerPlayer))
		{
			OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
		}
		DecreaseFillProgress();
	}
}

void AMS_CapturePlatform::StartFillProgress()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(StartProgressTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(StartProgressTimer);
	}
	if (GetWorld()->GetTimerManager().IsTimerActive(DecreaseProgressTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DecreaseProgressTimer);
	}

	GetWorld()->GetTimerManager().SetTimer(StartProgressTimer, this, &AMS_CapturePlatform::OnTickProgressTimer,
	                                       ProgressFillRate, true);
}

void AMS_CapturePlatform::StopFillProgress()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(StartProgressTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(StartProgressTimer);
	}
}

void AMS_CapturePlatform::DecreaseFillProgress()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(StartProgressTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(StartProgressTimer);
	}
	if (GetWorld()->GetWorld()->GetTimerManager().IsTimerActive(DecreaseProgressTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DecreaseProgressTimer);
	}

	GetWorld()->GetTimerManager().SetTimer(DecreaseProgressTimer, this,
	                                       &AMS_CapturePlatform::OnTickDecreaseProgressTimer,
	                                       ProgressDecreaseRate, true);
}

bool AMS_CapturePlatform::CheckProgressCondition()
{
	return PlayersInArea.Num() == 1;
}

void AMS_CapturePlatform::GiveCapturedPointReward()
{
	const UMS_RewardSettings* RewardSettings = GetDefault<UMS_RewardSettings>();
	if (!RewardSettings) return;

	if (IsValid(CurrentConquerPlayer))
	{
		AMS_PlayerCharacter* PlayerCharacter = CurrentConquerPlayer;
		if (!PlayerCharacter) return;

		AMS_PlayerState* RewardPlayerState = Cast<AMS_PlayerState>(PlayerCharacter->GetPlayerState());
		if (!RewardPlayerState) return;

		UMS_RewardSubsystem* RewardSubsystem = GetWorld()->GetSubsystem<UMS_RewardSubsystem>();
		if (!RewardSubsystem) return;

		RewardSubsystem->AddScore(RewardPlayerState, RewardSettings->CapturePointScore);
		UE_LOG(LogTemp, Warning, TEXT("Capture point reward"));
	}
}

void AMS_CapturePlatform::GiveHoldingPointReward()
{
	const UMS_RewardSettings* RewardSettings = GetDefault<UMS_RewardSettings>();
	if (!RewardSettings) return;

	if (IsValid(CurrentConquerPlayer))
	{
		AMS_PlayerCharacter* PlayerCharacter = CurrentConquerPlayer;
		if (!PlayerCharacter) return;

		AMS_PlayerState* RewardPlayerState = Cast<AMS_PlayerState>(PlayerCharacter->GetPlayerState());
		if (!RewardPlayerState) return;

		UMS_RewardSubsystem* RewardSubsystem = GetWorld()->GetSubsystem<UMS_RewardSubsystem>();
		if (!RewardSubsystem) return;

		RewardSubsystem->AddScore(RewardPlayerState, RewardSettings->HoldingPointScore);
		UE_LOG(LogTemp, Warning, TEXT("Holding point reward"));
	}
}

void AMS_CapturePlatform::OnTickProgressTimer()
{
	const bool bWasFull = IsFull();

	CurrentProgress = FMath::Clamp(CurrentProgress + ProgressFillStep, 0.0f, MaxProgress);
	OnProgressChanged.Broadcast(GetCurrentProgress());

	if (!bWasFull && IsFull())
	{
		GetWorld()->GetTimerManager().SetTimer(HoldingPointTimer, this, &AMS_CapturePlatform::OnPointHeld,
		                                       HoldingPointRate, true);
		GiveCapturedPointReward();
		GetWorld()->GetTimerManager().ClearTimer(StartProgressTimer);
	}
	UE_LOG(LogTemp, Warning, TEXT("Current progress: %f"), GetCurrentProgress());
}

void AMS_CapturePlatform::OnTickDecreaseProgressTimer()
{
	CurrentProgress = FMath::Clamp(CurrentProgress - ProgressDecreaseStep, 0.0f, MaxProgress);
	OnProgressChanged.Broadcast(GetCurrentProgress());

	if (CurrentProgress == 0.0f)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(DecreaseProgressTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(DecreaseProgressTimer);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Current progress: %f"), GetCurrentProgress());
}

void AMS_CapturePlatform::OnPointHeld()
{
	GiveHoldingPointReward();
}

void AMS_CapturePlatform::OnRep_ConquerPlayer()
{
	if (IsValid(CurrentConquerPlayer))
		OnConqueredBy.Broadcast(GetCurrentConquerPlayer());
}

void AMS_CapturePlatform::OnRep_CurrentProgress()
{
	OnProgressChanged.Broadcast(GetCurrentProgress());
}
