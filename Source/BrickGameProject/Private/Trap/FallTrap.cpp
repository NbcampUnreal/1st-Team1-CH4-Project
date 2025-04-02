// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap/FallTrap.h"

AFallTrap::AFallTrap()
{
	InitCollision(false, true);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
