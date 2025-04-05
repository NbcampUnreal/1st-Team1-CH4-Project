// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/FloorButtonSet.h"
#include "CooperatingDevice/FloorButton.h"
#include "GameFramework/Character.h"

// Sets default values
AFloorButtonSet::AFloorButtonSet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFloorButtonSet::CheckButtonStatus()
{
	if (!Button1 || !Button2) return;

	TArray<ACharacter*> PlayerOnButton1 = Button1->OverlappingPlayers;
	TArray<ACharacter*> PlayerOnButton2 = Button2->OverlappingPlayers;

	if (PlayerOnButton1.Num() == 1 && PlayerOnButton2.Num() == 1)
	{
		ACharacter* Player1 = PlayerOnButton1[0];
		ACharacter* Player2 = PlayerOnButton2[0];

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ButtonActive!!!!"));
	}
}

