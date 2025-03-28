// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/SlipTrap.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

ASlipTrap::ASlipTrap()
{
    SlipperyMaterial = CreateDefaultSubobject<UPhysicalMaterial>(TEXT("SlipperyMaterial"));

    if (SlipperyMaterial)
    {
        // ������ ���߱� (�̲������� ȿ��)
        SlipperyMaterial->Friction = 0.05f; // �⺻ ���� 0.05 (0�� �������� �̲�����)
        SlipperyMaterial->Restitution = 0.0f; // ƨ��� ���� (0 = ƨ���� ����)
        //SlipperyMaterial->FrictionCombineMode = EFrictionCombineMode::Type::Override;
        //SlipperyMaterial->FrictionCombineMode = EFrictionCombineMode::Override; // ������ �� ���� ���

        // ���� ��Ƽ���� ����
        StaticMeshComp->SetPhysMaterialOverride(SlipperyMaterial);
    }
}

void ASlipTrap::BeginPlay()
{
    Super::BeginPlay();

    if (StaticMeshComp && SlipperyMaterial)
    {
        StaticMeshComp->SetPhysMaterialOverride(SlipperyMaterial);
    }
}

void ASlipTrap::OperateTrap(ACharacter* Target)
{
    ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
    if (PlayerCharacter)
    {
        UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
        if (MovementComp)
        {
            MovementComp->bUseSeparateBrakingFriction = false; // ���� ���� ����
            MovementComp->BrakingDecelerationWalking = 0.0f;  // �ȱ� ���� ����
        }
    }
}
