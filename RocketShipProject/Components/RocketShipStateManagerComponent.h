// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketShipStateManagerComponent.generated.h"


class URocketShipStateBase;

/**
 * This component handles the different state behaviors of the Ship
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROCKETSHIPPROJECT_API URocketShipStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URocketShipStateManagerComponent();

	UFUNCTION()
	void HandleLaunch();

	UFUNCTION()
	void HandleStageBurnComplete();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                               FActorComponentTickFunction* ThisTickFunction) override;

	void SetState(const TSubclassOf<URocketShipStateBase>& NewStateClass);


protected:

	UPROPERTY(Transient)
	URocketShipStateBase* ShipState;
};
