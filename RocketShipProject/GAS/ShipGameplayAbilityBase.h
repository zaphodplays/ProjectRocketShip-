// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShipGameplayAbilityBase.generated.h"

/**
 * Base GameplayAbility for all Ship abilities
 */
UCLASS(Abstract)
class ROCKETSHIPPROJECT_API UShipGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
};
