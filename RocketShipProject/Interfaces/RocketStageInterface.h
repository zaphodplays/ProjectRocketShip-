// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RocketStageInterface.generated.h"

class IRocketShipInterface;
class IRocketStageModule;

UENUM(BlueprintType)
enum class EStageState : uint8
{
	Idle        UMETA(DisplayName = "Free"),
	Attached   UMETA(DisplayName = "Attached"),
	Active     UMETA(DisplayName = "Active"),
	InActive      UMETA(DisplayName = "InActive"),
	Spent        UMETA(DisplayName = "Spent"),
};
// This class does not need to be modified.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint), BlueprintType)
class URocketStageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This is the Interface for RocketStage , Multiple of these can be added to a RocketShip
 * Each RocketStage can be attached/detached to the Ship and provides a Thrust
 */
class ROCKETSHIPPROJECT_API IRocketStageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable)
	virtual EStageState GetCurrentState() = 0;

	UFUNCTION(BlueprintCallable)
	virtual UStaticMeshComponent* GetMesh() = 0;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCOM() = 0;

	UFUNCTION(BlueprintCallable)
	virtual void SetBaseMesh(UStaticMeshComponent* BMesh) = 0;

	UFUNCTION(BlueprintCallable)
	virtual FName& GetUpSocketName() = 0;

	UFUNCTION(BlueprintCallable)
	virtual FName& GetDownSocketName() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsFloating() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual void SetFloating(bool status) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateStage(bool status) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void DetachStage(bool status) = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsDetached() = 0;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetDetachVelocity() = 0;

	UFUNCTION(BlueprintCallable)
	virtual void SetDetachVelocity(FVector vVelocity) = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsActive() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsSpent() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool AttachToStage(TScriptInterface<IRocketStageInterface> OtherStage) = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool DetachFromStage(TScriptInterface<IRocketStageInterface> OtherStage) = 0;

	virtual bool AttachToShip(TScriptInterface<IRocketShipInterface> Ship) = 0;

	virtual void DetachFromShip(TScriptInterface<IRocketShipInterface> Ship) = 0;

	// rocket physics related

	UFUNCTION(BlueprintCallable)
	virtual float GetTotalFuelCapacity() = 0;

	UFUNCTION(BlueprintCallable)
	virtual float GetRemainingFuel() = 0;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCurrentThrust() = 0;

	UFUNCTION(BlueprintCallable)
	virtual float GetCurrentMass() = 0;

};
