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
        // 마찰력 낮추기 (미끄러지는 효과)
        SlipperyMaterial->Friction = 0.05f; // 기본 마찰 0.05 (0에 가까울수록 미끄러움)
        SlipperyMaterial->Restitution = 0.0f; // 튕기는 정도 (0 = 튕기지 않음)
        //SlipperyMaterial->FrictionCombineMode = EFrictionCombineMode::Type::Override;
        //SlipperyMaterial->FrictionCombineMode = EFrictionCombineMode::Override; // 무조건 이 값을 사용

        // 물리 머티리얼 적용
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
            MovementComp->bUseSeparateBrakingFriction = false; // 제동 마찰 제거
            MovementComp->BrakingDecelerationWalking = 0.0f;  // 걷기 감속 제거
        }
    }
}
