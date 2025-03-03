// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketShipProject/RocketShip.h"
#include "RocketShipProject/State/RocketShipIdleState.h"
#include "RocketShipProject/State/RocketShipLaunchedState.h"
#include "RocketShipProject/State/RocketShipStagingState.h"
#include "RocketShipStateManagerComponent.h"


class ARocketShip;
// Sets default values for this component's properties
URocketShipStateManagerComponent::URocketShipStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void URocketShipStateManagerComponent::HandleLaunch()
{
	SetState(URocketShipLaunchedState::StaticClass());
}

void URocketShipStateManagerComponent::HandleStageBurnComplete()
{
	SetState(URocketShipStagingState::StaticClass());
}


// Called when the game starts
void URocketShipStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetState(URocketShipIdleState::StaticClass());
	if (ARocketShip* Ship = Cast<ARocketShip>(GetOwner()))
	{
		Ship->OnLaunchTriggered.AddDynamic(this, &URocketShipStateManagerComponent::HandleLaunch);
		Ship->OnStageBurnComplete.AddDynamic(this, &URocketShipStateManagerComponent::HandleStageBurnComplete);
		//Ship->OnStageDetached.AddDynamic(this, &URocketStateManagerComponent::HandleStageDetached);
	}
	
}


// Called every frame
void URocketShipStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(ShipState) && GetOwner()->HasAuthority())
	{
		ShipState->Update(DeltaTime);
	}
}

void URocketShipStateManagerComponent::SetState(const TSubclassOf<URocketShipStateBase>& NewStateClass)
{
	if (IsValid(ShipState))
	{
		ShipState->Exit();
	}
	
	ShipState = NewObject<URocketShipStateBase>(this, NewStateClass);

	if (IsValid(ShipState))
	{
		ShipState->Enter(this->GetOwner());
	}
}


