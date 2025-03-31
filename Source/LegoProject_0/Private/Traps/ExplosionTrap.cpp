// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AExplosionTrap::AExplosionTrap()
{
    // ���� �ݰ�� ���� �� ������ ���� RadialForceComponent ����
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = ExplosionRadius;
    RadialForceComp->bImpulseVelChange = true; // �������� �� ����
    RadialForceComp->bIgnoreOwningActor = true; // �ڱ� �ڽ��� ���� ���� ����
    RadialForceComp->ImpulseStrength = 1500.0f;
}

void AExplosionTrap::ActiveTrap(ACharacter* Target)
{
    // ���� ȿ�� �߻�
    RadialForceComp->FireImpulse();

    // ���� ���� ���� ĳ���� ����
    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {
            // �÷��̾ ���� ȿ���� ���� �� �ֵ��� ����
            UPrimitiveComponent* CharacterMesh = Cast<UPrimitiveComponent>(Character->GetRootComponent());
            if (CharacterMesh && CharacterMesh->IsSimulatingPhysics())
            {
                FVector Direction = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                CharacterMesh->AddImpulse(Direction * ExplosionForce, NAME_None, true);
            }
        }
    }

    // ��ź ����
    Destroy();
}
