// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlock/BlueEffectBlock.h"
#include "GameFramework/Character.h"

ABlueEffectBlock::ABlueEffectBlock()
{
	BlockLifeTime = 3.0f;
	EffectDuration = 3.0f;
	InvincibilityTime = 3.0f;
}

void ABlueEffectBlock::ApplyEffect(ACharacter* Target)
{
	if (HasAuthority())
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
		if (PlayerCharacter)
		{
			// TODO : ���� ���� �����

			FTimerHandle EffectTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [PlayerCharacter]()
				{
					if (PlayerCharacter)
					{
						// TODO : ���� ���·� ���ƿ�
					}
				}, EffectDuration, false);
		}
	}
}
