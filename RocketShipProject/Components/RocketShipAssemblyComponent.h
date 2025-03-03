// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketShipAssemblyComponent.generated.h"


class ARocketShip;
class ARocketStage;

/**
 * This component can be attached to the player controller and provides drag and assemble functionality
 * for the RocketStage attachment to the Ship
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROCKETSHIPPROJECT_API URocketShipAssemblyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URocketShipAssemblyComponent();

	UFUNCTION(BlueprintCallable)
	void AddStageToShip(ARocketStage* Stage,
	ARocketShip* Ship);

	UFUNCTION(Server, Reliable)
	void ServerAddStageToShip(ARocketStage* Stage, ARocketShip* Ship);
	
	UFUNCTION(Server, Reliable)
	void DragActor(AActor* TargetActor);

	AActor* GetActorUnderCrosshair() const;

	void StartDragging();

	void StopDragging();

	UFUNCTION()
	void OnRep_DraggedComponent(const bool bIsDraggingOld);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	USceneComponent* DraggedComponent;

	UPROPERTY(ReplicatedUsing = OnRep_DraggedComponent)
	bool bIsDragging;
	
};
