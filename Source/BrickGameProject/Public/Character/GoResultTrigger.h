#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoResultTrigger.generated.h"

class UBoxComponent;

UCLASS()
class BRICKGAMEPROJECT_API AGoResultTrigger : public AActor
{
    GENERATED_BODY()

public:
    AGoResultTrigger();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY()
    TArray<APawn*> OverlappedPlayers;

    int32 TriggerCount;
    int32 RequiredPlayerCount;

    UPROPERTY()
    bool bReadyToCount = false;

};
