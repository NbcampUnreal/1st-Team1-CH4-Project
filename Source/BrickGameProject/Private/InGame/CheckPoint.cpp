#include "InGame/CheckPoint.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "GameFramework/Character.h"
//Components
#include "Components/BoxComponent.h"


ACheckPoint::ACheckPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
    TriggerVolume->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

void ACheckPoint::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character)
    {
        ABrickGamePlayerState* PS = Character->GetPlayerState<ABrickGamePlayerState>();
        if (PS)
        {
            PS->SetCurrentCheckPoint(GetActorLocation());
        }
    }
}

