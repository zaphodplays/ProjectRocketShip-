// Fill out your copyright notice in the Description page of Project Settings.
#include "RocketShipStagingState.h"
#include "RocketShipProject/Interfaces/RocketShipInterface.h"



void URocketShipStagingState::Enter(TScriptInterface<IRocketShipInterface> Ship)
{
	Super::Enter(Ship);
	Ship->AuthPerformStaging();
}

void URocketShipStagingState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	//deactivation timer
	//stage detach
	//activation timer
}
