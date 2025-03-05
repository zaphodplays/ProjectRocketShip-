// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShipPlayerController.h"
#include "RocketShip.h"
#include "Components/RocketShipAssemblyComponent.h"


ARocketShipPlayerController::ARocketShipPlayerController()
{
	ShipAssemblyComponent = CreateDefaultSubobject<URocketShipAssemblyComponent>("ShipAssemblyComponent");
}

void ARocketShipPlayerController::StartDragging()
{
	ShipAssemblyComponent->StartDragging();
}

void ARocketShipPlayerController::StopDragging()
{
	ShipAssemblyComponent->StopDragging();
}

void ARocketShipPlayerController::LaunchShip_Implementation(ARocketShip* Ship)
{
	if (!HasAuthority())
		return;
	if (!IsValid(Ship))
		return;
	//this->PlayerCameraManager->SetViewTarget(Ship);
	//Ship->Auth_SetIgnition(true);
	Ship->SetOwner(this);
	Ship->SetShipState(FGameplayTag::RequestGameplayTag(FName("RSP.Ship.State.Launch")));
}



void ARocketShipPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}


