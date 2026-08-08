// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MS_GraphicsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UMS_GraphicsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Saves the current scalability settings and applies the lowest preset at 25% render resolution. */
	UFUNCTION(BlueprintCallable, Category="Graphics")
	static void MakeGraphicsBad();

	/** Restores the settings saved by MakeGraphicsBad. Does nothing if there is no saved state. */
	UFUNCTION(BlueprintCallable, Category="Graphics")
	static void MakeGraphicsGood();
};
