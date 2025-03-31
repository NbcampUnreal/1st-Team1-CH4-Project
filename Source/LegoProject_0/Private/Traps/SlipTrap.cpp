// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/SlipTrap.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/CharacterMovementComponent.h"
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
            // 기본 제동 변수 저장
            OriginGroundFriction = MovementComp->GroundFriction;
            OriginBreakingDecelerationWalking = MovementComp->BrakingDecelerationWalking;
            
            // 미끄러지도록 제동 변경
            MovementComp->bUseSeparateBrakingFriction = false; // 제동 마찰 제거
            MovementComp->BrakingDecelerationWalking = 0.0f;  // 걷기 감속 제거
            MovementComp->GroundFriction = SlipTrapGroundFriction; // 지면 마찰력 제거
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
            // 미끄러지도록 제동 변경
            MovementComp->bUseSeparateBrakingFriction = true; // 제동 마찰 제거
            MovementComp->BrakingDecelerationWalking = OriginBreakingDecelerationWalking;  // 걷기 감속 제거
            MovementComp->GroundFriction = OriginGroundFriction; // 지면 마찰력 제거
        }
    }
}
