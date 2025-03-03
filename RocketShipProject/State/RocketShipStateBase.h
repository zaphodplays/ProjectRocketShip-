// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RocketShipStateBase.generated.h"

class IRocketShipInterface;
/**
 * 
 */
UCLASS(Abstract)
class ROCKETSHIPPROJECT_API URocketShipStateBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void Enter(TScriptInterface<IRocketShipInterface> Ship);

	virtual void Update(float DeltaTime){}

	virtual void Exit(){}

protected:
	TScriptInterface<IRocketShipInterface> RocketShip;
};

