// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlocks/RedBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

ARedBlock::ARedBlock()
{
	EffectDuration = 5.0f;
	BlockLifeTime = 5.0f;
	SpeedBoostValue = 1.2f;
	ImpulseValue = 2000.0f;
}

void ARedBlock::ApplyEffect(ACharacter* Target)
{
	if (HasAuthority())
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
		if (PlayerCharacter)
		{
			UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
			if (MovementComp)
			{
				// 기본 속도 저장
				float OriginalSpeed = MovementComp->MaxWalkSpeed;

				// 새로운 속도
				float NewSpeed = OriginalSpeed * SpeedBoostValue;

				// LaunchCharacter를 사용하여 앞으로 밀어줌
				FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() * ImpulseValue;
				PlayerCharacter->LaunchCharacter(LaunchDirection, true, true);
				
				// 새로운 속도 적용
				MovementComp->MaxWalkSpeed = NewSpeed;

				// 일정 시간 새로운 속도를 유지하다 원래대로 돌리기
				FTimerHandle EffectTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [PlayerCharacter, OriginalSpeed]()
					{
						if (PlayerCharacter)
						{
							UCharacterMovementComponent* ResetMovementComp = PlayerCharacter->GetCharacterMovement();
							if (ResetMovementComp)
							{
								ResetMovementComp->MaxWalkSpeed = OriginalSpeed; // 원래 속도
							}
						}
					}, EffectDuration, false);
			}
		}
	}

}