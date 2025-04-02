// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AExplosionTrap::AExplosionTrap()
{
    // Collision 초기화
    InitCollision(true, false);

    // 폭발 반경과 물리 힘 적용을 위한 RadialForceComponent 설정
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = ExplosionRadius;
    RadialForceComp->bImpulseVelChange = true; // 순간적인 힘 가함
    RadialForceComp->bIgnoreOwningActor = true; // 자기 자신은 영향 받지 않음
    RadialForceComp->ImpulseStrength = ExplosionForce;
}

void AExplosionTrap::ActiveTrap(ACharacter* Target)
{
    // 폭탄의 메쉬를 비활성화하여 보이지 않게 하기
    if (StaticMeshComp)
    {
        StaticMeshComp->SetVisibility(false);  // 메쉬 숨기기
        StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 충돌 비활성화
    }
    InitCollision(false, false); // 충돌 비활성화

    // 폭발 효과 발생 (Niagara System 추가)
    if (ExplosionNiagaraSystem)
    {
        // 폭발 위치에 Niagara System 스폰
        UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionNiagaraSystem,
            GetActorLocation(),
            GetActorRotation()
        );

        // Niagara 이펙트 종료 후 트랩 객체를 제거하도록 설정
        if (NiagaraComponent)
        {
            NiagaraComponent->OnSystemFinished.AddDynamic(this, &AExplosionTrap::OnExplosionFinished);
        }
    }

    // 폭발 효과 발생
    RadialForceComp->FireImpulse();
}

void AExplosionTrap::OnExplosionFinished(UNiagaraComponent* FinishedComponent)
{
    // 트랩 객체를 삭제
    Destroy();
}
