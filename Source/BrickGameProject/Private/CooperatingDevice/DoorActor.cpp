// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = StaticMeshComp;

}

void ADoorActor::SetDoorVisible(bool bIsVisible)
{
	StaticMeshComp->SetVisibility(bIsVisible, true);
	StaticMeshComp->SetCollisionEnabled(bIsVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

