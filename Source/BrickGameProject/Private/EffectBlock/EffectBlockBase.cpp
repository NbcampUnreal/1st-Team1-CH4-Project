// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlock/EffectBlockBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AEffectBlockBase::AEffectBlockBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component(Scene Component)
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComp);

	// Collision
	CollisionComp = nullptr;

	// Static Mesh Component
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetupAttachment(SceneComp);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AEffectBlockBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(BlockLifeTimeTimerHandle, this, &AEffectBlockBase::DestoryBlock, BlockLifeTime, false);
}

void AEffectBlockBase::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Charaacter = Cast<ACharacter>(OtherActor);
	if (Charaacter)
	{
		ApplyEffect(Charaacter);
	}
}

void AEffectBlockBase::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEffectBlockBase::ApplyEffect(ACharacter* Target)
{

}

void AEffectBlockBase::DestoryBlock()
{
	Destroy();
}

void AEffectBlockBase::InitCollsion(bool bIsBox)
{
	if (bIsBox)
	{
		UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		CollisionComp = Box;
	}
	else
	{
		USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
		CollisionComp = Sphere;
	}

	if (CollisionComp)
	{
		CollisionComp->SetupAttachment(SceneComp);

		CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEffectBlockBase::OnCharacterOverlap);
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AEffectBlockBase::OnCharacterEndOverlap);
	}
}

