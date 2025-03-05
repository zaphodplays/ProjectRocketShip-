// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipStagingController.h"

#include "RocketThrustComponent.h"
#include "Net/UnrealNetwork.h"
#include "RocketShipProject/RocketShip.h"
#include "RocketShipProject/RocketStage.h"
#include "RocketShipProject/Interfaces/RocketShipInterface.h"
#include "RocketShipProject/Interfaces/RocketStageInterface.h"


// Sets default values for this component's properties
UShipStagingController::UShipStagingController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	SetIsReplicatedByDefault(true);
	NumberOfAttachedStages = 0;
	// ...
}

void UShipStagingController::PerformStaging()
{
	if (ActivateTimer.IsValid() || DetachTimer.IsValid() )
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Performing Staging"));
	if (TScriptInterface<IRocketStageInterface> CurrentStage = Ship->GetAttachedStages().Last(); IsValid(CurrentStage.GetObject()))
	{
		CurrentStage->ActivateStage(false);
		GetWorld()->GetTimerManager().SetTimer(DetachTimer, this, &UShipStagingController::AuthDetachCurrentStage, DetachDelay, false);
	}
}

// Called when the game starts
void UShipStagingController::BeginPlay()
{
	Super::BeginPlay();
	if (Ship = (GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("StagingController::BeginPlay"));
	}
	
}

void UShipStagingController::AuthActivateCurrentStage()
{
	if (!AttachedStages.IsEmpty())
	{
		if (TScriptInterface<IRocketStageInterface>& CurrentStage = AttachedStages[NumberOfAttachedStages - 1])
		{
			CurrentStage->ActivateStage(true);
			Ship->UpdateCOM();
			Ship->GetThrusterComponent()->SetComponentTickEnabled(true);
		}
		
	}
	ActivateTimer.Invalidate();
}

void UShipStagingController::HandleStageBurn()
{
	if (ARocketShip* RocketShip = Cast<ARocketShip>(Ship.GetObject()))
	{
		RocketShip->OnStageBurnComplete.Broadcast();
	}
}

void UShipStagingController::AuthDetachCurrentStage()
{
	if (NumberOfAttachedStages <= 0)
	{
		DetachTimer.Invalidate();
		return;
	}
		
	TScriptInterface<IRocketStageInterface>& CurrentStage = AttachedStages[NumberOfAttachedStages - 1];
	if (AActor* StageActor = Cast<AActor>(CurrentStage.GetObject()); IsValid(StageActor))
	{
		CurrentStage->ActivateStage(false);
		StageActor->SetLifeSpan(ActivateDelay); 
	}
	AuthRemoveStage(CurrentStage);
	GetWorld()->GetTimerManager().SetTimer(ActivateTimer, this, &UShipStagingController::AuthSetNextStageToThrust, ActivateDelay, false);
	DetachTimer.Invalidate();
	
}

void UShipStagingController::AuthRemoveStage(TScriptInterface<IRocketStageInterface> RemovingStage)
{
	if (RemovingStage && IsValid(RemovingStage.GetObjectRef()) && AttachedStages.Contains(RemovingStage))
	{
		
		RemovingStage->DetachFromShip(this->GetOwner());
		
		AttachedStages.Pop(EAllowShrinking::Yes);
		Ship->UpdateCOM();
		NumberOfAttachedStages--;
		if (NumberOfAttachedStages == 0)
		{
			//PrepareForLanding();
		}
	}
}

void UShipStagingController::AuthAddStage(TScriptInterface<IRocketStageInterface> NewStage)
{
	if (!NewStage)
	{
		return;
	}
	if (!this->GetOwner()->HasAuthority())
	{
		return;
	}
	
	if (ARocketStage* NewRocketStage = Cast<ARocketStage>(NewStage.GetInterface()); IsValid(NewRocketStage) ) 
	{
		
		NewStage->SetFloating(false);
		NewRocketStage->SetOwner(this->GetOwner());
		NewStage->AttachToShip(Ship);
	
		AttachedStages.Emplace(NewStage);
		Ship->GetShipMesh()->SetSimulatePhysics(true);
		NumberOfAttachedStages++;
		Ship->UpdateCOM();
		
		UE_LOG(LogTemp, Warning, TEXT("Stage Attached::New Ship Mass: %f"), Ship->GetShipMesh()->GetMass());

	}

}

void UShipStagingController::OnRep_AttachedStages(const TArray<TScriptInterface<IRocketStageInterface>>& OldAttachedStages)
{
	if (!AttachedStages.IsEmpty() && AttachedStages.Num() > OldAttachedStages.Num())
	{
		int NewElementsNum = AttachedStages.Num() - OldAttachedStages.Num();
		
		Ship->UpdateCOM();
			
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Blue, FString::Printf(TEXT("ShipMass = %f"), Ship->GetShipMesh()->GetMass()) );
		DrawDebugCircle(this->GetWorld(), Ship->GetShipMesh()->GetCenterOfMass(), 200, 360, FColor::MakeRedToGreenColorFromScalar(AttachedStages.Num()/7.f), true, 10.0, 0, 10.0, FVector(1,0,0), FVector(0,1,0));
		
	}
}

void UShipStagingController::OnRep_NumberOfAttachedStages(const uint32 OldNumOfAttachedStages)
{
	if (NumberOfAttachedStages - OldNumOfAttachedStages == 1)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ShipMass = %f"), ShipMesh->GetMass()) );
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Green, FString::Printf(TEXT("Attached Stage = %d"), NumberOfAttachedStages+1));
		//DrawDebugCircle(this->GetWorld(), ShipMesh->GetCenterOfMass(), 200, 360, FColor::MakeRedToGreenColorFromScalar(NumberOfAttachedStages/7.f), true, 10.0, 0, 10.0, FVector(1,0,0), FVector(0,1,0));
	}
	else if (NumberOfAttachedStages - OldNumOfAttachedStages == -1)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Detached Stage = %d"), OldNumOfAttachedStages));
		DrawDebugCircle(this->GetWorld(), Ship->GetShipMesh()->GetCenterOfMass(), 200, 360, FColor::Green, true, 10.0, 0, 10.0, FVector(1,0,0), FVector(0,1,0));
	}
}

void UShipStagingController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShipStagingController, AttachedStages);
	DOREPLIFETIME(UShipStagingController, NumberOfAttachedStages);
}

void UShipStagingController::AuthSetNextStageToThrust()
{
	Ship->SetShipState(FGameplayTag::RequestGameplayTag(FName(TEXT("RSP.Ship.State.Thrusting"))));
}
