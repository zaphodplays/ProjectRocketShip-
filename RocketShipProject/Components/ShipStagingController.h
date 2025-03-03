// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketShipProject/Interfaces/StagingControllerInterface.h"
#include "ShipStagingController.generated.h"


class IRocketShipInterface;
class IRocketStageInterface;

/**
 * Component that handles the RocketStages attached to the Ship
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROCKETSHIPPROJECT_API UShipStagingController :
public UActorComponent,
public IStagingControllerInterface
{

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShipStagingController();

	void PerformStaging();

	void AuthRemoveStage(TScriptInterface<IRocketStageInterface> RemovingStage);

	void AuthAddStage(TScriptInterface<IRocketStageInterface> NewStage);

	void AuthDetachCurrentStage();

	void AuthActivateCurrentStage();

	virtual void HandleStageBurn() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnRep_AttachedStages(const TArray<TScriptInterface<IRocketStageInterface>>& OldAttachedStages);

	UFUNCTION()
	void OnRep_NumberOfAttachedStages(uint32 OldNumOfAttachedStages);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttachedStages)
	TArray<TScriptInterface<IRocketStageInterface> > AttachedStages;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_NumberOfAttachedStages)
	uint32 NumberOfAttachedStages;

	FTimerHandle DetachTimer;

	FTimerHandle ActivateTimer;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float DetachDelay;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float ActivateDelay;

	bool bIsTimerRunning;

	UPROPERTY(Transient)
	TScriptInterface<IRocketShipInterface> Ship;

};



