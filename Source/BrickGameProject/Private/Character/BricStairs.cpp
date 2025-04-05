#include "Character/BricStairs.h"

ABricStairs::ABricStairs()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    StairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StairMesh"));
    StairMesh->SetupAttachment(RootComponent);

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABricStairs::OnTriggerBegin);
}

void ABricStairs::BeginPlay()
{
    Super::BeginPlay();

    TargetRelativeLocation = StairMesh->GetRelativeLocation(); // 최종 위치 = 현재 상대 위치
    StartRelativeLocation = TargetRelativeLocation - FVector(0.f, 0.f, 300.f); // 시작 위치 = 아래로 300

    StairMesh->SetRelativeLocation(StartRelativeLocation); // 계단을 아래로 숨김
}

void ABricStairs::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldMove)
    {
        FVector CurrentLocation = StairMesh->GetRelativeLocation();
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetRelativeLocation, DeltaTime, MoveSpeed);
        StairMesh->SetRelativeLocation(NewLocation);

        if (FVector::Dist(NewLocation, TargetRelativeLocation) < 1.f)
        {
            bShouldMove = false;
        }
    }
}

void ABricStairs::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        bShouldMove = true;
    }
}
