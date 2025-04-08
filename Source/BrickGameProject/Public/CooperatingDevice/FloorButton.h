// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorButton.generated.h"

class UCapsuleComponent;
class AFloorButtonSet;

UCLASS()
class BRICKGAMEPROJECT_API AFloorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorButton();

	// 컴포넌트들
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshBaseComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshButtonComp;

	UPROPERTY()
	TArray<ACharacter*> OverlappingPlayers;

	// 버튼 애니메이션
	UPROPERTY(EditDefaultsOnly)
	float MoveDuration = 0.2f;

	// 버튼 Material
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DefaultMaterial;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* PressedMaterial;
	

protected:
	float PressDepth = 15.0f;
	FVector InitialButtonLocation;
	FVector PressedButtonLocation;
	bool bIsPressed = false;
	bool bLocked = false;

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void UpdateButtonState();

protected:
	void PlayPressAnimation();
	void PlayReleaseAnimation();
	void SetButtonMaterial(UMaterialInterface* NewMaterial);
};
