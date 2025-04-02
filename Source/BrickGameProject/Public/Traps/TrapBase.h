// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class UShapeComponent;

UCLASS()
class BRICKGAMEPROJECT_API ATrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapBase();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UShapeComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	virtual void OnTrapOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTrapEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void ActiveTrap(ACharacter* Target);
	virtual void DeactiveTrap(ACharacter* Target);

	void InitCollision(bool bUseCollision, bool bIsBox);

};
