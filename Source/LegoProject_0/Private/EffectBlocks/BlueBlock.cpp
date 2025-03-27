// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlocks/BlueBlock.h"
#include "GameFramework/Character.h"

ABlueBlock::ABlueBlock()
{
	BlockLifeTime = 3.0f;
	EffectDuration = 3.0f;
	InvincibilityTime = 3.0f;
}

void ABlueBlock::ApplyEffect(ACharacter* Target)
{
	if (HasAuthority())
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
		if (PlayerCharacter)
		{
			// TODO : 무적 상태 만들기

			FTimerHandle EffectTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [PlayerCharacter]()
				{
					if (PlayerCharacter)
					{
						// TODO : 원래 상태로 돌아옴
					}
				}, EffectDuration, false);
		}
	}
}
