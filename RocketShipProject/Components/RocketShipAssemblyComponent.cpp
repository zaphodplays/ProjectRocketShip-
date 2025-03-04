// Fill out your copyright notice in the Description page of Project Settings.
#include "RocketShipAssemblyComponent.h"
#include "RocketShipProject/RocketShip.h"
#include "RocketShipProject/RocketShipProjectCharacter.h"
#include "RocketShipProject/RocketStage.h"
#include "RocketShipProject/Interfaces/RocketStageInterface.h"



// Sets default values for this component's properties
URocketShipAssemblyComponent::URocketShipAssemblyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsDragging = false;
	// ...
}

// Called when the game starts
void URocketShipAssemblyComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void URocketShipAssemblyComponent::AddStageToShip(ARocketStage* Stage,
	ARocketShip* Ship)
{

	if (!IsValid(Ship))
		return;

	if (!IsValid(Stage))
		return;

	if (!IsValid(Ship->GetOwner()))
	{
		Ship->SetOwner(this->GetOwner());
	}
	ServerAddStageToShip(Stage, Ship);
}

void URocketShipAssemblyComponent::ServerAddStageToShip_Implementation(ARocketStage* Stage, ARocketShip* Ship)
{
	if (!GetOwner()->HasAuthority()) return;
	//Stage->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Stage->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Ship->AuthAddStage(Stage);
}

AActor* URocketShipAssemblyComponent::GetActorUnderCrosshair() const
{
	UE_CLOG(GetOwner()->GetLocalRole() == ROLE_Authority, LogTemp, Error, TEXT("%s should be called on client"), ANSI_TO_TCHAR(__FUNCTION__));
	// Get screen center
	int32 ScreenWidth, ScreenHeight;
	const APlayerController* Controller = Cast<APlayerController>(GetOwner());
	Controller->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	// Convert screen position to world direction
	FVector WorldLocation, WorldDirection;
	Controller->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

	FHitResult HitResult;
	FVector Start = Controller->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (WorldDirection * 2000.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Controller->GetPawn());
	DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, false, 1.0, 0, 3.0);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, Params))
	{
		return HitResult.GetActor();
	}
	return nullptr;
}

void URocketShipAssemblyComponent::StartDragging()
{
	if (GetOwner()->HasAuthority()) return;
	AActor* TargetActor = GetActorUnderCrosshair();
	if (!TargetActor) return;
    DragActor(TargetActor);
	
}

void URocketShipAssemblyComponent::StopDragging()
{
	
	if (bIsDragging && DraggedComponent)
	{
		UStaticMeshComponent* SM = DraggedComponent->GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (IsValid(SM))
		{
			//turn physics off
			//SM->SetEnableGravity(true);
			//SM->SetSimulatePhysics(true);
			
		}
		DraggedComponent->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (IsValid(SM))
		{
			//turn physics off
			//SM->SetEnableGravity(true);
			//SM->SetSimulatePhysics(true);
			
		}
		
		//DraggedComponent->GetOwner()->SetActorLocation(this->GetPawn()->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
		
	}
	bIsDragging = false;
	DraggedComponent = nullptr;
	
}

void URocketShipAssemblyComponent::DragActor_Implementation(AActor* TargetActor)
{
	if (!GetOwner()->HasAuthority()) return;
	DraggedComponent = Cast<USceneComponent>(TargetActor->GetRootComponent());
	APlayerController* Controller = Cast<APlayerController>(GetOwner());
	ARocketShipProjectCharacter* MyCharacter = Cast<ARocketShipProjectCharacter>(Controller->GetPawn());
	if (IsValid(DraggedComponent) && IsValid(MyCharacter))
	{
		UStaticMeshComponent* SM = DraggedComponent->GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (IsValid(SM))
		{
			//turn physics off
			SM->SetEnableGravity(false);
			SM->SetSimulatePhysics(false);
		}
		if (IRocketStageInterface* RocketStageActor = Cast<IRocketStageInterface>(TargetActor))
		{
			RocketStageActor->SetFloating(true);
		}
		bIsDragging = DraggedComponent->AttachToComponent(MyCharacter->GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform);
		DraggedComponent->SetRelativeLocation(FVector(500, 0, 100));
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Green, "Dragging Failed");
	}
}

void URocketShipAssemblyComponent::OnRep_DraggedComponent(const bool bIsDraggingOld)
{
	const FString DragMessage = bIsDragging ? "Dragging":"Dropping"; 
	GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Green, *DragMessage);
}

void URocketShipAssemblyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URocketShipAssemblyComponent, bIsDragging);
}