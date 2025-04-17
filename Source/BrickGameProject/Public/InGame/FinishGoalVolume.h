#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Network_Structure/EGameTeam.h"
#include "FinishGoalVolume.generated.h"

class UBoxComponent;

UCLASS()
class BRICKGAMEPROJECT_API AFinishGoalVolume : public AActor
{
	GENERATED_BODY()

public:
	AFinishGoalVolume();



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FinishGoalVolume")
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameTeam Team;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};