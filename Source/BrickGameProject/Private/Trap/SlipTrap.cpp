// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap/SlipTrap.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

ASlipTrap::ASlipTrap()
{
    InitCollision(true, true);
    SlipTrapGroundFriction = 0.0f;
}

void ASlipTrap::BeginPlay()
{
    Super::BeginPlay();
}

void ASlipTrap::ActiveTrap(ACharacter* Target)
{
    Super::ActiveTrap(Target);

    ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
    if (PlayerCharacter)
    {
        UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            // 원래 값 저장
            OriginGroundFriction = MovementComp->GroundFriction;
            OriginBreakingDecelerationWalking = MovementComp->BrakingDecelerationWalking;

            // 미끄러지는 값 적용
            MovementComp->bUseSeparateBrakingFriction = false;
            MovementComp->BrakingDecelerationWalking = 0.0f;
            MovementComp->GroundFriction = SlipTrapGroundFriction;
        }

        // 넘어지는 몽타주 재생
        if (FallDownMontage)
        {
            PlayerCharacter->PlayAnimMontage(FallDownMontage);
        }

        // 3초 후 일어나는 몽타주 재생
        if (GetWorld())
        {
            FTimerHandle TimerHandle;
            FTimerDelegate TimerDelegate;

            TimerDelegate.BindLambda([PlayerCharacter, this]()
                {
                    if (PlayerCharacter && StandUpMontage)
                    {
                        PlayerCharacter->PlayAnimMontage(StandUpMontage);
                    }
                });

            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.0f, false);
        }
    }
}


void ASlipTrap::DeactiveTrap(ACharacter* Target)
{
    Super::DeactiveTrap(Target);

    ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
    if (PlayerCharacter)
    {
        UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            // �̲�������� ���� ����
            MovementComp->bUseSeparateBrakingFriction = true; // ���� ���� ����
            MovementComp->BrakingDecelerationWalking = OriginBreakingDecelerationWalking;  // �ȱ� ���� ����
            MovementComp->GroundFriction = OriginGroundFriction; // ���� ������ ����
        }
    }
}
