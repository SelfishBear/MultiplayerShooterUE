// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MS_Pickable.generated.h"

UINTERFACE()
class UMS_Pickable : public UInterface
{
	GENERATED_BODY()
};

class MULTIPLAYERSHOOTER_API IMS_Pickable
{
	GENERATED_BODY()

public:
	virtual void Pickup(ACharacter* PickingCharacter) = 0;
};
