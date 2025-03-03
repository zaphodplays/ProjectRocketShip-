// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShipStateBase.h"

void URocketShipStateBase::Enter(TScriptInterface<IRocketShipInterface> Ship)
{
	RocketShip = Ship;
}
