// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlocks/GreenBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"

AGreenBlock::AGreenBlock()
{
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent); // 루트에 부착

	BlockLifeTime = 3.0f;
	JumpBoostValue = 1.5f;
}

void AGreenBlock::ApplyEffect(ACharacter* Target)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (PlayerCharacter)
	{
		UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->AddImpulse(FVector(0.0f, 0.0f, 1000.0f), true);
		}
	}
}
