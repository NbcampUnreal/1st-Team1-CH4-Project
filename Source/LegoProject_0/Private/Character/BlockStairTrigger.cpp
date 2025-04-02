#include "Character/BlockStairTrigger.h"
#include "Components/BoxComponent.h"
#include "Character/LegoCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlockStairTrigger::ABlockStairTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ABlockStairTrigger::BeginPlay()
{
    Super::BeginPlay();
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABlockStairTrigger::OnTriggerOverlap);
}

// 트리거에 캐릭터가 들어왔을 때 실행
void ABlockStairTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasBuiltBridge) return; // ✅ 이미 다리를 만든 경우 무시

    if (Cast<ALegoCharacter>(OtherActor))
    {
        bHasBuiltBridge = true;

        CurrentIndex = 0;

        ForwardDir = OtherActor->GetActorForwardVector().GetSafeNormal();

        
        float ForwardStartOffset = 200.f; 

        StartLocation = GetActorLocation() + ForwardDir * ForwardStartOffset;

        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle, this, &ABlockStairTrigger::SpawnBridgeStep,
            StepDelay, true
        );
    }

}

void ABlockStairTrigger::SpawnBridgeStep()
{
    if (CurrentIndex >= NumSteps)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        return;
    }

    FVector ForwardDir2D = FVector(ForwardDir.X, ForwardDir.Y, 0.f).GetSafeNormal();
    FVector Right = FVector::CrossProduct(FVector::UpVector, ForwardDir2D);

    float BlockZOffset = -45.f; // ✅ 바닥에 착 붙게 조정

    for (int32 x = -BridgeWidth / 2; x <= BridgeWidth / 2; ++x)
    {
        FVector RightOffset = Right * BlockSpacing * x;
        FVector ForwardOffset = ForwardDir2D * BlockSpacing * CurrentIndex;

        FVector SpawnLocation = StartLocation + ForwardOffset + RightOffset + FVector(0, 0, BlockZOffset);

        AActor* Spawned = GetWorld()->SpawnActor<AActor>(BlockToSpawn, SpawnLocation, FRotator::ZeroRotator);

        if (Spawned)
        {
            TArray<UStaticMeshComponent*> MeshComps;
            Spawned->GetComponents<UStaticMeshComponent>(MeshComps);
            for (UStaticMeshComponent* Mesh : MeshComps)
            {
                Mesh->SetSimulatePhysics(false); // 중력 X
                Mesh->SetEnableGravity(false);
                Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            }
        }
    }

    ++CurrentIndex;
}



