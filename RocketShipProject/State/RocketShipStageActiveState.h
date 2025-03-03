// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RocketShipStateBase.h"
#include "RocketShipStageActiveState.generated.h"

/**
 * 
 */
UCLASS()
class ROCKETSHIPPROJECT_API URocketShipStageActiveState : public URocketShipStateBase
{
private:
	GENERATED_BODY()
	
public:
	
	virtual void Enter(TScriptInterface<IRocketShipInterface> Ship) override;
	
	virtual void Update(float DeltaTime) override;

};
