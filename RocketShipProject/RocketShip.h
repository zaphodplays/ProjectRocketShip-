// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RocketShipInterface.h"
#include "RocketShip.generated.h"

class URocketThrustComponent;
class UShipStagingController;
class URocketShipStateManagerComponent;
class IRocketStageInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageBurnComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageDetached);

/**
 * RocketShip - This is the main Ship actor that is built by assembling multiple Rocket Stages and burns these stages to achieve thrust and flight. 
 */
UCLASS(Blueprintable)
class ROCKETSHIPPROJECT_API ARocketShip :
public AActor,
public IRocketShipInterface
{

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARocketShip();

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bIgnition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URocketShipStateManagerComponent* RocketShipStateManager;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UShipStagingController* ShipStagingController;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URocketThrustComponent* ThrustComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Auth_SetIgnition(bool bIgnitionStatus);

	virtual UStaticMeshComponent* GetShipMesh() override;

	virtual TArray<TScriptInterface<IRocketStageInterface>>& GetAttachedStages() override;

	virtual TScriptInterface<IStagingControllerInterface> GetStagingController() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UStaticMeshComponent* ProxyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float StructureMass;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	FVector Destination;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float DetachDelay;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float ActivateDelay;

	bool bIsTimerRunning;

public:

	virtual FVector GetDestination() override;

	UFUNCTION(BlueprintCallable)
	virtual void AuthPerformStaging() override;

	UFUNCTION()
	virtual void AuthActivateStage() override;

	UFUNCTION()
	virtual void UpdateCOM() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchCameraToTop();

	UFUNCTION(BlueprintNativeEvent)
	void PrepareForLanding();

	UFUNCTION()
	virtual void AuthAddStage(TScriptInterface<IRocketStageInterface> NewStage) override;

private:

	FVector ComputeCOM(float& TotalMass);

	FTimerHandle DetachTimer;

	FTimerHandle ActivateTimer;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual URocketThrustComponent* GetThrusterComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnLaunchTriggered OnLaunchTriggered;

	UPROPERTY(BlueprintAssignable)
	FOnStageBurnComplete OnStageBurnComplete;

	UPROPERTY(BlueprintAssignable)
	FOnStageDetached OnStageDetached;
};

inline void ARocketShip::Auth_SetIgnition(bool bIgnitionStatus)
{
	if (HasAuthority() && !bIgnition)
	{
		bIgnition = bIgnitionStatus;
		ProxyMesh->SetHiddenInGame(true);
		ProxyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProxyMesh->SetSimulatePhysics(false);
		ProxyMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ShipMesh->SetEnableGravity(true);
		OnLaunchTriggered.Broadcast();
	}
}

