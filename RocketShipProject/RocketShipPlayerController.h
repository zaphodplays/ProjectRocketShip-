// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RocketShipPlayerController.generated.h"

class URocketShipAssemblyComponent;
class ARocketStage;
class IRocketStageInterface;
class ARocketShip;
/**
 * 
 */
UCLASS()
class ROCKETSHIPPROJECT_API ARocketShipPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARocketShipPlayerController();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LaunchShip(ARocketShip* Ship);

	void StartDragging();
	
	void StopDragging();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	URocketShipAssemblyComponent* ShipAssemblyComponent;

protected:
	virtual void SetupInputComponent() override;

};
