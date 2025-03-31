// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AExplosionTrap::AExplosionTrap()
{
    // Collision �ʱ�ȭ
    InitCollision(true, false);

    // ���� �ݰ�� ���� �� ������ ���� RadialForceComponent ����
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = ExplosionRadius;
    RadialForceComp->bImpulseVelChange = true; // �������� �� ����
    RadialForceComp->bIgnoreOwningActor = true; // �ڱ� �ڽ��� ���� ���� ����
    RadialForceComp->ImpulseStrength = ExplosionForce;
}

void AExplosionTrap::ActiveTrap(ACharacter* Target)
{
    // ���� ȿ�� �߻�
    RadialForceComp->FireImpulse();

    // ��ź ����
    Destroy();
}
