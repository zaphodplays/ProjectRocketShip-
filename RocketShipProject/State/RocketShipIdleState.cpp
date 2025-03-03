// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShipIdleState.h"

void URocketShipIdleState::Enter(TScriptInterface<IRocketShipInterface> Ship)
{
	Super::Enter(Ship);
	UE_LOG(LogTemp, Warning, TEXT("URocketShipIdleState::Enter"));
}
