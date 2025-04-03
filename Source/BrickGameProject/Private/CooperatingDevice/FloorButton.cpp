// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/FloorButton.h"

// Sets default values
AFloorButton::AFloorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

