// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShipGASet.h"

#include "Components/ShipAbilitySystemComponent.h"
#include "GAS/ShipGameplayAbilityBase.h"


void URocketShipGASet::GiveAbilities(UShipAbilitySystemComponent* AbilitySystemComponent) const
{
	for (const auto& [Command, ShipAbilityClass] : Abilities)
	{
		if (ShipAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShipAbilityClass, 1, (int32)Command));
		}
	}
}
