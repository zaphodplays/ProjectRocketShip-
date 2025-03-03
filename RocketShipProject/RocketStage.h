// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/RocketStageInterface.h"
#include "RocketStage.generated.h"

class UPhysicsConstraintComponent;

/**
 * RocketStage - This is the RocketStage actor that can be attached/detached to the Ship and provides a Thrust Vector to the Ship when activated
 */
UCLASS()
class ROCKETSHIPPROJECT_API ARocketStage :
public AActor,
public IRocketStageInterface
{

private:
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual FName& GetUpSocketName() override;

	UFUNCTION(BlueprintCallable)
	virtual FName& GetDownSocketName() override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCOM() override;

	UFUNCTION(BlueprintCallable)
	virtual UStaticMeshComponent* GetMesh() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetBaseMesh(UStaticMeshComponent* BMesh) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsFloating() const override;

	UFUNCTION(BlueprintCallable)
	virtual void SetFloating(bool status) override;

	UFUNCTION(BlueprintCallable)
	virtual bool AttachToStage(TScriptInterface<IRocketStageInterface> OtherStage) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DetachFromStage(TScriptInterface<IRocketStageInterface> OtherStage) override;

	UFUNCTION(BlueprintCallable)
	virtual bool AttachToShip(TScriptInterface<IRocketShipInterface> Ship) override;

	UFUNCTION(BlueprintCallable)
	virtual void DetachFromShip(TScriptInterface<IRocketShipInterface> Ship) override;


public:
	// Sets default values for this actor's properties
	ARocketStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Replicated)
	UStaticMeshComponent* Mesh;
	
	TWeakObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Physics")
	UPhysicsConstraintComponent* BottomConstraint;  

	UPROPERTY(EditAnywhere, Category = "Physics")
	UPhysicsConstraintComponent* TopConstraint; //Connects to the next stage

	float StageMass;
	FVector ThrustVector;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bIsFloating;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ActivationStatusChanged)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, Category = "RocketStage", ReplicatedUsing = "OnRep_SpentStatusChanged")
	bool bIsSpent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = "OnRep_DetachmentStatusChanged", Category = "RocketStage")
	bool bIsDetaching;

	UPROPERTY(EditAnywhere, Category = "RocketStage")
	float SimulationDelay;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "RocketStage")
	FVector DetachVelocity;

	UPROPERTY(EditAnywhere)
	FName SocketName_UP;

	UPROPERTY(EditAnywhere)
	FName SocketName_DOWN;
	

private:

	FTimerHandle KineticTimerHandle;

protected:

	UPROPERTY(VisibleAnywhere, Replicated)
	EStageState StageState;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float StructureMass;

	FVector CenterOfMass;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float FuelMass; // Initial fuel mass (kg)

	UPROPERTY(VisibleAnywhere, Category = "Rocket Stage")
	float FuelRemaining;

	UPROPERTY(VisibleAnywhere, Category = "Rocket Stage")
	float AbsThrust;

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float BurnRate; // Fuel consumption per second (kg/s)

	UPROPERTY(EditAnywhere, Category = "Rocket Stage")
	float SpecificImpulse; // ISP in seconds

	static constexpr float Gravity = 9.81f;

	float TimeSinceSeparation;

	FVector LocationAtSeparation;

	float CalculateThrust(float DeltaTime);

public:

	UFUNCTION(BlueprintCallable)
	virtual EStageState GetCurrentState() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetTotalFuelCapacity() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetRemainingFuel() override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCurrentThrust() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetCurrentMass() override;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateStage(bool status) override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsActive() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRep_ActivationStatusChanged(const bool bOldStatus);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRep_DetachmentStatusChanged(const bool bOldStatus);

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_SpentStatusChanged(const bool bOldStatus);

	UFUNCTION(BlueprintCallable)
	virtual bool IsSpent() override;

	UFUNCTION(BlueprintCallable)
	virtual void DetachStage(bool status) override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsDetached() override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetDetachVelocity() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetDetachVelocity(FVector vVelocity) override;
	
};

inline void ARocketStage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARocketStage, Mesh);
	DOREPLIFETIME(ARocketStage, StageState);
	DOREPLIFETIME(ARocketStage, bIsFloating);
	DOREPLIFETIME(ARocketStage, bIsActive);
	DOREPLIFETIME(ARocketStage, bIsSpent);
	DOREPLIFETIME(ARocketStage, bIsDetaching);
	DOREPLIFETIME(ARocketStage, DetachVelocity);
}
