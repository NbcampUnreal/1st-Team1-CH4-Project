#include "Character/GoResultTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"

AGoResultTrigger::AGoResultTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(100.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

    TriggerCount = 0;
    RequiredPlayerCount = 4; 
    bReadyToCount = false;
}

void AGoResultTrigger::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGoResultTrigger::OnOverlapBegin);

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FTimerHandle DelayHandle;
    GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]()
        {
            int32 Count = 0;
            for (TActorIterator<APawn> It(GetWorld()); It; ++It)
            {
                if (*It && It->IsPlayerControlled())
                {
                    Count++;
                }
            }

            RequiredPlayerCount = Count;
            bReadyToCount = true;

            // ✅ 이제 트리거 작동 가능하게 다시 켜기
            TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

            UE_LOG(LogTemp, Warning, TEXT("플레이어 수 확정: %d"), RequiredPlayerCount);

        }), 0.5f, false);
}



void AGoResultTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bReadyToCount) return;

    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        APawn* OverlappingPawn = Cast<APawn>(OtherActor);

        if (OverlappingPawn && !OverlappedPlayers.Contains(OverlappingPawn))
        {
            OverlappedPlayers.Add(OverlappingPawn);
            TriggerCount++;

            UE_LOG(LogTemp, Warning, TEXT("트리거 진입: %s (%d/%d)"), *OtherActor->GetName(), TriggerCount, RequiredPlayerCount);

            if (HasAuthority() && TriggerCount >= RequiredPlayerCount)
            {
                UE_LOG(LogTemp, Warning, TEXT("모든 플레이어가 도착했습니다. 결과 씬으로 이동!"));
                GetWorld()->ServerTravel(TEXT("/Game/Maps/ResultLevel?listen"));
                return; 
            }

        }
    }
}

