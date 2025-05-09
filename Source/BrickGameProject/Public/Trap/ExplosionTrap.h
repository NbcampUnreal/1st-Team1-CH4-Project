// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap/TrapBase.h"
#include "ExplosionTrap.generated.h"

class URadialForceComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BRICKGAMEPROJECT_API AExplosionTrap : public ATrapBase
{
	GENERATED_BODY()
	
public:
	AExplosionTrap();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float ExplosionForce = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Nigara Effect")
	UNiagaraSystem* ExplosionNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float ExplosionRadius = 500.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosionTrap|Animation")
	UAnimMontage* FallDownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosionTrap|Animation")
	UAnimMontage* StandUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosionTrap|Effect")
	UNiagaraSystem* StarEffectNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* ExplosionSound;




	virtual void ActiveTrap(ACharacter* Target) override;
	UFUNCTION()
	void OnExplosionFinished(UNiagaraComponent* FinishedComponent);

};
