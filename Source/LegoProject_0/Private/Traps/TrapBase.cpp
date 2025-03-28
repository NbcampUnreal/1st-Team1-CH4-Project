// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/TrapBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ATrapBase::ATrapBase()
{
	// Root Component(Scene Component)
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComp);

	// Box Component
	SphereColliderComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoxCollider"));
	SphereColliderComp->SetupAttachment(SceneComp);
	SphereColliderComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereColliderComp->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnCharacterOverlap);
	SphereColliderComp->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::OnCharacterEndOverlap);

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

void ATrapBase::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Charaacter = Cast<ACharacter>(OtherActor);
	if (Charaacter)
	{
		OperateTrap(Charaacter);
	}
}

void ATrapBase::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ATrapBase::OperateTrap(ACharacter* Target)
{
}

