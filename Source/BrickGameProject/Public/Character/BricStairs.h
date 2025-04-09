// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BricStairs.generated.h"

UCLASS()
class BRICKGAMEPROJECT_API ABricStairs : public AActor
{
    GENERATED_BODY()

public:
    ABricStairs();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StairMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere)
    float MoveSpeed = 100.f;

    FVector StartRelativeLocation;
    FVector TargetRelativeLocation;

    bool bShouldMove = false;

    UFUNCTION()
    void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};
