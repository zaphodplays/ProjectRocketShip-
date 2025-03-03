// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StagingControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UStagingControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This is the Interface for the Stage Controller of the Ship that handles the control of Rocket stages in the Ship
 */
class ROCKETSHIPPROJECT_API IStagingControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void HandleStageBurn() = 0;
};
