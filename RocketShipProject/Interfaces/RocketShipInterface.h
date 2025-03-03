// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RocketShipInterface.generated.h"

class URocketThrustComponent;
class IStagingControllerInterface;
class IRocketStageInterface;

// This class does not need to be modified.
UINTERFACE()
class URocketShipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This is the main API for the Rocket Ship
 */
class ROCKETSHIPPROJECT_API IRocketShipInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UStaticMeshComponent* GetShipMesh() = 0;

	virtual FVector GetDestination() = 0;

	virtual TArray<TScriptInterface<IRocketStageInterface>>& GetAttachedStages() = 0;

	virtual TScriptInterface<IStagingControllerInterface> GetStagingController() = 0;

	virtual URocketThrustComponent* GetThrusterComponent() = 0;

	virtual void AuthPerformStaging() = 0;

	virtual void AuthActivateStage() = 0;

	virtual void UpdateCOM() = 0;

	virtual void AuthAddStage(TScriptInterface<IRocketStageInterface> NewStage) = 0;
};
