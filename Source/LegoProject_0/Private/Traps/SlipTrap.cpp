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
            // �⺻ ���� ���� ����
            OriginGroundFriction = MovementComp->GroundFriction;
            OriginBreakingDecelerationWalking = MovementComp->BrakingDecelerationWalking;
            
            // �̲��������� ���� ����
            MovementComp->bUseSeparateBrakingFriction = false; // ���� ���� ����
            MovementComp->BrakingDecelerationWalking = 0.0f;  // �ȱ� ���� ����
            MovementComp->GroundFriction = SlipTrapGroundFriction; // ���� ������ ����
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
            // �̲��������� ���� ����
            MovementComp->bUseSeparateBrakingFriction = true; // ���� ���� ����
            MovementComp->BrakingDecelerationWalking = OriginBreakingDecelerationWalking;  // �ȱ� ���� ����
            MovementComp->GroundFriction = OriginGroundFriction; // ���� ������ ����
        }
    }
}
