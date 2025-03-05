// Fill out your copyright notice in the Description page of Project Settings.
#include "RocketShipStageActiveState.h"
#include "RocketShipProject/Components/RocketThrustComponent.h"
#include "RocketShipProject/Interfaces/RocketShipInterface.h"
#include "RocketShipProject/Interfaces/RocketStageInterface.h"
#include "RocketShipProject/Interfaces/StagingControllerInterface.h"


void URocketShipStageActiveState::Enter(TScriptInterface<IRocketShipInterface> Ship)
{
	Super::Enter(Ship);
}

void URocketShipStageActiveState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (IsValid(RocketShip.GetObjectRef()) && !RocketShip->GetAttachedStages().IsEmpty()
		&& RocketShip->GetAttachedStages().Last()->IsActive())
	{
		if ( RocketShip->GetAttachedStages().Last()->IsSpent())
		{
			if (TScriptInterface<IStagingControllerInterface> StagingController = RocketShip->GetStagingController())
			{
				StagingController->HandleStageBurn();
			}
		}
		else
		{
			RocketShip->UpdateCOM();
			//RocketShip->GetThrusterComponent()->ApplyThrust();
		}
		
	}
	
}
