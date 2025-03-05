// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RocketShipGASet.generated.h"

class UShipAbilitySystemComponent;
class UShipGameplayAbilityBase;

UENUM(BlueprintType)
namespace EShipAbilityCmdBinds
{
	enum Type : int
	{
		Ability1				UMETA(DisplayName = "Ability1 (LMB)"),
		Ability2				UMETA(DisplayName = "Ability2 (RMB)"),
		Ability3				UMETA(DisplayName = "Ability3 (Q)"),
		Ability4				UMETA(DisplayName = "Ability4 (E)"),
		Ability5				UMETA(DisplayName = "Ability5 (R)"),
		Ability6				UMETA(DisplayName = "Ability6 (Shift)"),
		Ability7				UMETA(DisplayName = "Ability7 (Space)"),
		Ability8				UMETA(DisplayName = "Ability8 (B)"),
		Ability9				UMETA(DisplayName = "Ability9 (T)"),
	};
}

/**
 *	 struct that pairs a enum input command to a AbilityClass
 */
USTRUCT()
struct FShipAbilityBindInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = BindInfo)
	TEnumAsByte<EShipAbilityCmdBinds::Type>	Command = EShipAbilityCmdBinds::Ability1;;

	UPROPERTY(EditAnywhere, Category = BindInfo)
	TSubclassOf<UShipGameplayAbilityBase>	GameplayAbilityClass;
};

/**
 * 
 */
UCLASS()
class ROCKETSHIPPROJECT_API URocketShipGASet : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = ShipAbilitySet)
	TArray<FShipAbilityBindInfo>	Abilities;

public:
	
	void GiveAbilities(UShipAbilitySystemComponent* AbilitySystemComponent) const;
};
