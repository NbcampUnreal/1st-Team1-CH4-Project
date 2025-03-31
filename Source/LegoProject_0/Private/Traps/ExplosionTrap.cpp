// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

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
    // 폭발 효과 발생
    RadialForceComp->FireImpulse();

    // 폭탄 제거
    Destroy();
}
