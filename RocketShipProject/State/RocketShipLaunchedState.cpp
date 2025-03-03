// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketShipProject/Interfaces/RocketShipInterface.h"
#include "RocketShipLaunchedState.h"


void URocketShipLaunchedState::Enter(TScriptInterface<IRocketShipInterface> Ship)
{
	Super::Enter(Ship);
	UE_LOG(LogTemp, Warning, TEXT("Entering URocketShipLaunchedState"));
	
}

void URocketShipLaunchedState::Update(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Updating URocketShipLaunchedState"));
	RocketShip->AuthActivateStage();
}

