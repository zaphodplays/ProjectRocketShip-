// Fill out your copyright notice in the Description page of Project Settings.



#include "RocketThrustComponent.h"

#include "Conditions/MovieSceneCondition.h"
#include "RocketShipProject/Interfaces/RocketShipInterface.h"
#include "RocketShipProject/Interfaces/RocketStageInterface.h"


void URocketThrustComponent::OnRegister()
{
	Super::OnRegister();
	
}

void URocketThrustComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ApplyThrust();
}

// Sets default values for this component's properties
URocketThrustComponent::URocketThrustComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.Target = this;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	
	
}

void URocketThrustComponent::ApplyThrust()
{
	if (RocketShip->GetAttachedStages().IsEmpty())
	{
		return;
	}
	if (TScriptInterface<IRocketStageInterface> CurrentStage = RocketShip->GetAttachedStages().Last(); IsValid(CurrentStage.GetObject()))
	{
		RocketShip->UpdateCOM();
		if (CurrentStage->IsSpent())
		{
			SetComponentTickEnabled(false);
			RocketShip->SetShipState(FGameplayTag::RequestGameplayTag(FName("RSP.Ship.State.Staging")));
			return;
		}
		FVector Thrust = (RocketShip->GetDestination() - RocketShip->GetShipMesh()->GetCenterOfMass()).GetSafeNormal();
		Thrust = Thrust * CurrentStage->GetCurrentThrust().Length() * 100.f;
		
		if (Thrust.Length() > 0.f )
		{
			FVector PhysicsVelocity = RocketShip->GetShipMesh()->GetPhysicsLinearVelocity();
			FRotator TargetRotation = PhysicsVelocity.ToOrientationRotator();
			TargetRotation.Pitch += -90.f;
			FRotator InterpedRotator = FMath::RInterpConstantTo(this->GetOwner()->GetActorRotation(), TargetRotation, 0.001f, 1000.f);
			//this->GetOwner()->SetActorRotation(InterpedRotator, ETeleportType::TeleportPhysics);
			RocketShip->GetShipMesh()->AddForceAtLocation(Thrust, RocketShip->GetShipMesh()->GetCenterOfMass());
		}
	}
	
}


// Called when the game starts
void URocketThrustComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.RegisterTickFunction(GetOwner()->GetLevel());
	RocketShip = TScriptInterface<IRocketShipInterface>(GetOwner());
	
}


