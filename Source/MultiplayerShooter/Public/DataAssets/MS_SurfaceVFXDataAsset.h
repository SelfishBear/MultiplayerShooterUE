// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MS_SurfaceVFXDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FParticleData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit VFX")
	TObjectPtr<UParticleSystem> VFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit VFX")
	FVector InitialEffectScale = FVector(1.0f, 1.0f, 1.0f);
};

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UMS_SurfaceVFXDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit VFX")
	TMap<TEnumAsByte<EPhysicalSurface>, FParticleData> SurfaceEffects;
};
