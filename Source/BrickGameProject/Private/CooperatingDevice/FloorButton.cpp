// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/FloorButton.h"
#include "Components/CapsuleComponent.h"
#include "CooperatingDevice/FloorButtonSet.h"
#include "GameFramework/Character.h"

// Sets default values
AFloorButton::AFloorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Root Component(Scene Component)
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComp);

	// Collision Component
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleComp->SetupAttachment(SceneComp);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::OnOverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::OnOverlapEnd);

	// Static Mesh Component
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetupAttachment(SceneComp);
}

void AFloorButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		OverlappingPlayers.AddUnique(Player);
		UpdateButtonState();
	}
}

void AFloorButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		OverlappingPlayers.Remove(Player);
		UpdateButtonState();
	}
}

void AFloorButton::UpdateButtonState()
{
	if (ButtonSet)
	{
		ButtonSet->CheckButtonStatus();
	}
}

