// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
    // ��ź�� �޽��� ��Ȱ��ȭ�Ͽ� ������ �ʰ� �ϱ�
    if (StaticMeshComp)
    {
        StaticMeshComp->SetVisibility(false);  // �޽� �����
        StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // �浹 ��Ȱ��ȭ
    }
    InitCollision(false, false); // �浹 ��Ȱ��ȭ

    // ���� ȿ�� �߻� (Niagara System �߰�)
    if (ExplosionNiagaraSystem)
    {
        // ���� ��ġ�� Niagara System ����
        UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionNiagaraSystem,
            GetActorLocation(),
            GetActorRotation()
        );

        // Niagara ����Ʈ ���� �� Ʈ�� ��ü�� �����ϵ��� ����
        if (NiagaraComponent)
        {
            NiagaraComponent->OnSystemFinished.AddDynamic(this, &AExplosionTrap::OnExplosionFinished);
        }
    }

    // ���� ȿ�� �߻�
    RadialForceComp->FireImpulse();
}

void AExplosionTrap::OnExplosionFinished(UNiagaraComponent* FinishedComponent)
{
    // Ʈ�� ��ü�� ����
    Destroy();
}
