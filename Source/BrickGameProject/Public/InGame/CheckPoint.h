#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "CheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class BRICKGAMEPROJECT_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	UBoxComponent* TriggerVolume;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	USoundBase* CheckPointSound;
	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	TSubclassOf<AActor> CheckPointEffectClass;
	UPROPERTY()
	TSet<ACharacter*> TriggeredCharacters;
};
