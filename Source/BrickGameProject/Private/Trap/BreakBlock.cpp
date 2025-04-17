// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap/BreakBlock.h"

// Sets default values
ABreakBlock::ABreakBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;

	BlockMesh->SetSimulatePhysics(false);
	BlockMesh->SetNotifyRigidBodyCollision(true);
	BlockMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

}

// Called when the game starts or when spawned
void ABreakBlock::BeginPlay()
{
	Super::BeginPlay();

	BlockMesh->OnComponentHit.AddDynamic(this, &ABreakBlock::OnBlockHit);
	
}

// Called every frame
void ABreakBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakBlock::OnBlockHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(HitComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("💥 블록 충돌 감지됨!"));
		Mesh->SetSimulatePhysics(true);
	}
}