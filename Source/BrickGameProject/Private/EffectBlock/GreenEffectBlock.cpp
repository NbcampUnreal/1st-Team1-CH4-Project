// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlock/GreenEffectBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

AGreenEffectBlock::AGreenEffectBlock()
{
	InitCollsion(false);

	// Static Mesh Component
	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	StaticMeshComp2->SetupAttachment(StaticMeshComp);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent); // ��Ʈ�� ����

	BlockLifeTime = 3.0f;
	JumpBoostValue = 1.5f;
}

void AGreenEffectBlock::ApplyEffect(ACharacter* Target)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (PlayerCharacter)
	{
		UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->AddImpulse(FVector(0.0f, 0.0f, 1000.0f), true);
		}

		// 🔊 점프대 효과음 재생
		if (JumpPadSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, JumpPadSound, PlayerCharacter->GetActorLocation());
		}
	}
}

