// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockStairTrigger.generated.h"

UCLASS()
class LEGOPROJECT_0_API ABlockStairTrigger : public AActor
{
    GENERATED_BODY()

public:
    ABlockStairTrigger();

protected:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* TriggerBox;

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTriggerOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UPROPERTY(EditAnywhere, Category = "Bridge")
    TSubclassOf<AActor> BlockToSpawn;

    UPROPERTY(EditAnywhere, Category = "Bridge")
    int32 NumSteps = 10;

    UPROPERTY(EditAnywhere, Category = "Bridge")
    FVector StepOffset = FVector(150.f, 0.f, 0.f); 

    UPROPERTY(EditAnywhere, Category = "Bridge")
    float StepDelay = 0.3f;

    UPROPERTY(EditAnywhere, Category = "Bridge")
    float DropHeight = 100.f;


    UPROPERTY(EditAnywhere)
    int32 BridgeWidth = 3; 

    UPROPERTY(EditAnywhere, Category = "Bridge")
    float BlockSpacing = 50.f;  

    bool bHasBuiltBridge = false;



private:
    FVector StartLocation;
    FVector ForwardDir;
    int32 CurrentIndex;
    FTimerHandle TimerHandle;

    void SpawnBridgeStep(); 

};
