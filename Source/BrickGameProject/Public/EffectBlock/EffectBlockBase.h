// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectBlockBase.generated.h"

class UBoxComponent;

UCLASS()
class BRICKGAMEPROJECT_API AEffectBlockBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffectBlockBase();
	UPROPERTY(VisibleAnywhere, Category = "Root")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UShapeComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComp;

	float BlockLifeTime;
	FTimerHandle BlockLifeTimeTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void OnCharacterOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCharacterEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void ApplyEffect(ACharacter* Target);
	void DestoryBlock();
	void InitCollsion(bool bIsBox);
};
