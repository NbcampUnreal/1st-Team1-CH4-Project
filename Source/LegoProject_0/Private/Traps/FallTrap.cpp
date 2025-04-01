// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/FallTrap.h"

AFallTrap::AFallTrap()
{
	InitCollision(false, true);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
