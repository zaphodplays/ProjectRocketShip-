// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShipGASet.h"
#include "ShipGameplayAbilityBase.h"
#include "RocketShipProject/Components/ShipAbilitySystemComponent.h"


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
