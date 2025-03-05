
// Fill out your copyright notice in the Description page of Project Settings.
UE_DISABLE_OPTIMIZATION
#include "RocketShip.h"
#include "Components/ShipStagingController.h"
#include "Components/RocketShipStateManagerComponent.h"
#include "Components/RocketThrustComponent.h"
#include "RocketStage.h"
#include "State/RocketShipStageActiveState.h"
#include "RocketShipGASet.h"
#include "Components/ShipAbilitySystemComponent.h"


// Sets default values
ARocketShip::ARocketShip()
{
	ProxyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProxyMesh");
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShipMesh");
	ShipAbilitySystemComponent = CreateDefaultSubobject<UShipAbilitySystemComponent>("AbilitySystemComponent");
	RocketShipStateManager = CreateDefaultSubobject<URocketShipStateManagerComponent>("RocketShipStateManager");
	ShipStagingController = CreateDefaultSubobject<UShipStagingController>("ShipStagingController");
	ThrustComponent = CreateDefaultSubobject<URocketThrustComponent>("ThrustComponent");
	RootComponent = ShipMesh;
	ProxyMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	bIgnition = false;
	bReplicates = true;
	ShipMesh->SetIsReplicated(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ARocketShip::BeginPlay()
{
	Super::BeginPlay();
	
	ShipMesh->SetMassOverrideInKg(NAME_None, StructureMass, true);
	if (!HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage( 1, 500.f, FColorList::Bronze, FString::Printf(TEXT("RocketShip Project: Amar Shukla")));
		return;
	}

	if (!IsValid(ShipAbilitySystemComponent) || !IsValid(AbilityDataAsset) )
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent or AbilityDataAsset Invalid"));
		return;
	}

	AbilityDataAsset->GiveAbilities(ShipAbilitySystemComponent);

	// Start in Idle state
	SetShipState(FGameplayTag::RequestGameplayTag(FName("RSP.Ship.State.Idle")));
}

void ARocketShip::Auth_SetIgnition(bool bIgnitionStatus)
{
	if (HasAuthority() && !bIgnition)
	{
		bIgnition = bIgnitionStatus;
		//ProxyMesh->SetHiddenInGame(true);
		ProxyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProxyMesh->SetSimulatePhysics(false);
		ProxyMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ShipMesh->SetEnableGravity(true);
		//OnLaunchTriggered.Broadcast();
		SetShipState(FGameplayTag::RequestGameplayTag(FName("RSP.Ship.State.Thrusting")));
	}
}

UStaticMeshComponent* ARocketShip::GetShipMesh()
{
	return ShipMesh;
}

TArray<TScriptInterface<IRocketStageInterface>>& ARocketShip::GetAttachedStages()
{
	return ShipStagingController->AttachedStages;
}

TScriptInterface<IStagingControllerInterface> ARocketShip::GetStagingController()
{
	return ShipStagingController;
}

URocketThrustComponent* ARocketShip::GetThrusterComponent()
{
	return ThrustComponent;
}

// Called every frame
void ARocketShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIgnition && !HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage( 1, 0.3f, FColor::Green, FString::Printf(TEXT("Current Height = %f km"), ShipMesh->GetComponentLocation().Z/100000.f));
		GEngine->AddOnScreenDebugMessage(2, 0.3f, FColor::Yellow, FString::Printf(TEXT("Current Velocity = %f km/s"), ShipMesh->GetComponentVelocity().Z/100000.f));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Previous Rocket Mass: %f"), ShipMesh->GetMass());
	
}

const FGameplayTag& ARocketShip::GetShipState()
{
	return CurrentStateTag;
}

FVector ARocketShip::GetDestination()
{
	return Destination;
}

void ARocketShip::AuthPerformStaging()
{
	if (HasAuthority())
	{
		ShipStagingController->PerformStaging();
	}
}

void ARocketShip::TriggerGameplayEvent(const FGameplayTag& EventTag)
{
	if (ShipAbilitySystemComponent)
	{
		FGameplayEventData EventData;
		EventData.EventTag = EventTag;
		ShipAbilitySystemComponent->HandleGameplayEvent(EventTag, &EventData);
	}
}

void ARocketShip::SetShipState(const FGameplayTag& NewStateTag)
{
	if (HasAuthority() && ShipAbilitySystemComponent )
	{
		ShipAbilitySystemComponent->RemoveLooseGameplayTag(CurrentStateTag);
		ShipAbilitySystemComponent->AddLooseGameplayTag(NewStateTag);
		CurrentStateTag = NewStateTag;
		TriggerGameplayEvent(CurrentStateTag);
	}
}

void ARocketShip::AuthActivateStage()
{
	if (HasAuthority())
	{
		ShipStagingController->AuthActivateCurrentStage();
		//RocketShipStateManager->SetState(URocketShipStageActiveState::StaticClass());
	}
}


void ARocketShip::PrepareForLanding_Implementation()
{
	
}

void ARocketShip::AuthAddStage(TScriptInterface<IRocketStageInterface> NewStage)
{
	if (!NewStage)
	{
		return;
	}
	if (!HasAuthority())
	{
		return;
	}
	ShipStagingController->AuthAddStage(NewStage);

}

FVector ARocketShip::ComputeCOM(float& TotalMass)
{
	FVector WeightedSum = FVector::ZeroVector;
	TotalMass = StructureMass;
	for (auto Stage : ShipStagingController->AttachedStages)
	{
		
		float Mass = Stage->GetCurrentMass();
		WeightedSum += (Stage->GetCOM() - GetRootComponent()->GetComponentLocation()) * Mass;
		TotalMass += Mass;
	}

	FVector COM =  (TotalMass > 0) ? (WeightedSum / TotalMass) : FVector::ZeroVector;
	//COM += RootComponent->GetComponentLocation();
	COM.X = RootComponent->GetComponentLocation().X;
	COM.Y = RootComponent->GetComponentLocation().Y;
	return COM;
}

void ARocketShip::OnRep_StateTagChanged(const FGameplayTag OldTag)
{
	GEngine->AddOnScreenDebugMessage(5, 0.3f, FColor::Magenta, FString::Printf(TEXT("State changed from %s to %s"), *OldTag.ToString(), *CurrentStateTag.ToString()));
}

void ARocketShip::UpdateCOM()
{
	if (ShipMesh)
	{
		float TotalMass = 0.f;
		FVector NewCenterOfMass = ComputeCOM(TotalMass);
		UE_LOG(LogTemp, Warning, TEXT("UpdateCOM:: setting New Ship Mass: %f"), TotalMass);
		ShipMesh->SetMassOverrideInKg(NAME_None, TotalMass, true);
		//ShipMesh->SetCenterOfMass(NewCenterOfMass);
	}
}

void ARocketShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARocketShip, bIgnition);
	DOREPLIFETIME(ARocketShip, ShipMesh);
	DOREPLIFETIME(ARocketShip, ProxyMesh);
	DOREPLIFETIME(ARocketShip, CurrentStateTag);
}



