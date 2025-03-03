// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RocketShipStateBase.h"
#include "RocketShipCruisingState.generated.h"

/**
 * 
 */
UCLASS()
class ROCKETSHIPPROJECT_API URocketShipCruisingState : public URocketShipStateBase
{
public:
	virtual void Enter(TScriptInterface<IRocketShipInterface> Ship) override;
	
	virtual void Update(float DeltaTime) override;

private:
	GENERATED_BODY()
};
