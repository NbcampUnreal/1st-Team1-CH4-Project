// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/TrapBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrapBase::ATrapBase()
{
	// Root Component(Scene Component)
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComp);

	// Collision Component
	CollisionComponent = nullptr;

	// Static Mesh Component
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetupAttachment(SceneComp);

	bReplicates = true;

}

// Called when the game starts or when spawned
void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrapBase::OnTrapOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Charaacter = Cast<ACharacter>(OtherActor);
	if (Charaacter)
	{
		ActiveTrap(Charaacter);
	}
}

void ATrapBase::OnTrapEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Charaacter = Cast<ACharacter>(OtherActor);
	if (Charaacter)
	{
		DeactiveTrap(Charaacter);
	}
}

void ATrapBase::ActiveTrap(ACharacter* Target)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap!!!!"));
}

void ATrapBase::DeactiveTrap(ACharacter* Target)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End Overlap!!!!"));
}

void ATrapBase::InitCollision(bool bUseCollision, bool bIsBox)
{
	if (!bUseCollision)
	{
		CollisionComponent = nullptr;
		return;
	}

	if (bIsBox)
	{
		UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		CollisionComponent = Box;
	}
	else
	{
		USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
		CollisionComponent = Sphere;
	}

	if (CollisionComponent)
	{
		CollisionComponent->SetupAttachment(RootComponent);

		CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnTrapOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::OnTrapEndOverlap);
	}
}

