// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketThrustComponent.generated.h"


class IRocketShipInterface;

/**
 * This component handles the Thrust movement of the Ship 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROCKETSHIPPROJECT_API URocketThrustComponent : public UActorComponent
{
protected:
	virtual void OnRegister() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URocketThrustComponent();

	void ApplyThrust();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TScriptInterface<IRocketShipInterface> RocketShip;

};
