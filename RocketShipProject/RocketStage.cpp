// Fill out your copyright notice in the Description page of Project Settings.
UE_DISABLE_OPTIMIZATION
#include "RocketStage.h"
#include "Interfaces/RocketShipInterface.h"
#include "Interfaces/RocketStageInterface.h"



FName& ARocketStage::GetUpSocketName()
{
	return SocketName_UP;
}

FName& ARocketStage::GetDownSocketName()
{
	return SocketName_DOWN;
}

FVector ARocketStage::GetCOM()
{
	CenterOfMass = Mesh->GetCenterOfMass();// - RootComponent->GetComponentLocation();
	return CenterOfMass;
}

UStaticMeshComponent* ARocketStage::GetMesh()
{
	return Mesh;
}

void ARocketStage::SetBaseMesh(UStaticMeshComponent* BMesh)
{
	BaseMesh = BMesh;
}

bool ARocketStage::IsFloating() const
{
	return bIsFloating;
}

void ARocketStage::SetFloating(bool Status)
{
	bIsFloating = Status;
}

bool ARocketStage::AttachToStage(TScriptInterface<IRocketStageInterface> OtherStage)
{
	if (!HasAuthority()) return false;
	if (bIsFloating) return false;
	if (!OtherStage || !OtherStage->GetMesh()) return false;
	if (!OtherStage->IsFloating()) return false;
	
	// Align the next stage with this stage's top socket
	if (OtherStage->GetMesh()->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetDownSocketName()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ARocketStage::SnapToStage" + GetLocalRole());
	}
	
	OtherStage->GetMesh()->WeldTo(BaseMesh.Get(), GetDownSocketName(), true);
	OtherStage->SetFloating(false);
	
	OtherStage->SetBaseMesh(BaseMesh.Get());
	return true;
}

bool ARocketStage::DetachFromStage(TScriptInterface<IRocketStageInterface> OtherStage)
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	Mesh->SetMassOverrideInKg(NAME_None, GetCurrentMass(), true);
	
	UPrimitiveComponent* OriginalActorComp = Cast<UPrimitiveComponent>(OtherStage->GetMesh()->GetAttachmentRoot());
	Mesh->SetPhysicsLinearVelocity(OriginalActorComp->GetPhysicsLinearVelocity()*0.9995f);

	SetNetUpdateFrequency(400.f);
	SetMinNetUpdateFrequency(200.f);
	ForceNetUpdate();
		
	//------continue from here
	GetMesh()->SetEnableGravity(true);
	return true;
}

// Sets default values
ARocketStage::ARocketStage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = Mesh;
	
	bIsFloating = false;
	Mesh->SetSimulatePhysics(true);
	BaseMesh = Mesh;
	
	bReplicates = true;
	SetReplicateMovement(true);
	Mesh->SetIsReplicated(true);
	
	FuelRemaining = FuelMass;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

// Called when the game starts or when spawned
void ARocketStage::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetMassOverrideInKg(NAME_None, 0.001f, true);
	CenterOfMass = Mesh->GetCenterOfMass() - RootComponent->GetComponentLocation();
	FuelRemaining = FuelMass;
	
	//FVector COM = GetCOM() + RootComponent->GetComponentLocation();
	DrawDebugCircle(this->GetWorld(), Mesh->GetCenterOfMass(), 200, 360, FColor::Yellow, true, 10.0, 0, 2.0, FVector(1,0,0), FVector(0,1,0));
}


void ARocketStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsActive && HasAuthority())
	{
		AbsThrust = CalculateThrust(DeltaTime);
		
	}
	else if (bIsActive && !HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(4, 0.3f, FColor::Yellow, FString::Printf(TEXT("Current Stage Fuel Percent = %d"), FuelRemaining/FuelMass*100.f));
	}
	
}

EStageState ARocketStage::GetCurrentState()
{
	return StageState;
}


float ARocketStage::GetTotalFuelCapacity()
{
	return FuelMass;
}

float ARocketStage::GetRemainingFuel()
{
	return FuelRemaining;
}

FVector ARocketStage::GetCurrentThrust()
{
	return FVector(0,0, 1) * AbsThrust;
}

float ARocketStage::GetCurrentMass()
{
	return StructureMass + FuelRemaining;
}

void ARocketStage::ActivateStage(bool status)
{
	bIsActive = status;
	this->SetActorTickEnabled(true);
}

bool ARocketStage::IsActive()
{
	return bIsActive;
}

void ARocketStage::OnRep_DetachmentStatusChanged_Implementation(const bool bOldStatus)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColorList::Firebrick, bIsDetaching ? TEXT("Detaching Stage") : TEXT("Stage Not Detaching"));
	//GetMesh()->SetSimulatePhysics(false);
	//SetActorTickEnabled(true);
}

void ARocketStage::OnRep_ActivationStatusChanged_Implementation(const bool bOldStatus)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, bIsActive ? TEXT("Active Stage") : TEXT("Stage Deactivated"));
}


void ARocketStage::OnRep_SpentStatusChanged_Implementation(const bool bOldStatus)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, bIsSpent ? TEXT("Stage Spent") : TEXT("Stage Unspent"));
}

bool ARocketStage::IsSpent()
{
	return bIsSpent;
}

void ARocketStage::DetachStage(bool status)
{
	bIsDetaching = status;
	GetWorld()->GetTimerManager().SetTimer(
			KineticTimerHandle, 
			[this]()
			{
				SetActorTickEnabled(false);
				SetLifeSpan(SimulationDelay);
				GetMesh()->SetSimulatePhysics(true);
				GetMesh()->SetPhysicsLinearVelocity(DetachVelocity);
				ForceNetUpdate();
				KineticTimerHandle.Invalidate();
			}, 
			SimulationDelay, false);  // Delay for s
}

bool ARocketStage::AttachToShip(TScriptInterface<IRocketShipInterface> Ship)
{
	auto WeldStage = [this, &Ship](UStaticMeshComponent* FromMesh, UStaticMeshComponent* ToMesh)
	{
		FromMesh->AttachToComponent(ToMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetDownSocketName());
		FromMesh->WeldTo(Ship->GetShipMesh(), GetDownSocketName(), true);
		SetBaseMesh(Ship->GetShipMesh());
	};
	SetFloating(false);
	SetOwner(this);
	if (Ship->GetAttachedStages().IsEmpty())
	{
		
		WeldStage(GetMesh(), Ship->GetShipMesh());
		
		UE_LOG(LogTemp, Warning, TEXT("Stage Attached::New Ship Mass: %f"), Ship->GetShipMesh()->GetMass());

		return true;
	}

	TScriptInterface<IRocketStageInterface>& CurrentStage = Ship->GetAttachedStages().Last();
	WeldStage(Mesh, CurrentStage->GetMesh());
	return true;
	
}

void ARocketStage::DetachFromShip(TScriptInterface<IRocketShipInterface> Ship)
{
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	Mesh->SetMassOverrideInKg(NAME_None, GetCurrentMass(), true);
	UPrimitiveComponent* OriginalActorComp = Cast<UPrimitiveComponent>(Ship->GetShipMesh());
	Mesh->SetPhysicsLinearVelocity(OriginalActorComp->GetPhysicsLinearVelocity()*0.9995f);
	SetNetUpdateFrequency(500.f);
	SetMinNetUpdateFrequency(200.f);
	ForceNetUpdate();
	Mesh->SetEnableGravity(true);
}


bool ARocketStage::IsDetached()
{
	return bIsDetaching;
}

FVector ARocketStage::GetDetachVelocity()
{
	return DetachVelocity;
}

void ARocketStage::SetDetachVelocity(FVector vVelocity)
{
	DetachVelocity = vVelocity;
}

float ARocketStage::CalculateThrust(float DeltaTime)
{
	if (bIsActive && FuelRemaining <= 0)
	{
		bIsSpent = true;
		this->SetActorTickEnabled(false);
		return 0.0f; // No fuel, no thrust
	}

	float ExhaustVelocity = SpecificImpulse * Gravity; // ISP * g
	float Thrust = BurnRate * ExhaustVelocity; // F = m_dot * v_e
	FuelRemaining -= BurnRate * DeltaTime;
	return Thrust;
}
