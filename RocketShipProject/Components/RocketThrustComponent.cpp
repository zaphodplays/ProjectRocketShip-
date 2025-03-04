// Fill out your copyright notice in the Description page of Project Settings.



#include "RocketThrustComponent.h"
#include "RocketShipProject/Interfaces/RocketShipInterface.h"


// Sets default values for this component's properties
URocketThrustComponent::URocketThrustComponent()
{
	
}

void URocketThrustComponent::ApplyThrust(const float ThrustStrength)
{
	if (RocketShip->GetAttachedStages().IsEmpty())
	{
		return;
	}
	if (TScriptInterface<IRocketStageInterface> CurrentStage = RocketShip->GetAttachedStages().Last(); IsValid(CurrentStage.GetObject()))
	{
		FVector Thrust = (RocketShip->GetDestination() - RocketShip->GetShipMesh()->GetCenterOfMass()).GetSafeNormal();
		Thrust = Thrust * ThrustStrength * 100.f;
		
		if (Thrust.Length() > 0.f )
		{
			RocketShip->GetShipMesh()->AddForceAtLocation(Thrust, RocketShip->GetShipMesh()->GetCenterOfMass());
			FVector PhysicsVelocity = RocketShip->GetShipMesh()->GetPhysicsLinearVelocity();
			FRotator TargetRotation = Thrust.ToOrientationRotator();
			TargetRotation.Pitch += -90.f;
			FRotator InterpedRotator = FMath::RInterpConstantTo(this->GetOwner()->GetActorRotation(), TargetRotation, 0.001f, 1.f);
			
			//this->GetOwner()->SetActorRotation(InterpedRotator, ETeleportType::TeleportPhysics);
			
			//
		}
	}
	
}


// Called when the game starts
void URocketThrustComponent::BeginPlay()
{
	Super::BeginPlay();
	RocketShip = TScriptInterface<IRocketShipInterface>(GetOwner());
	
}


