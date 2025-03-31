// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AExplosionTrap::AExplosionTrap()
{
    // 폭발 반경과 물리 힘 적용을 위한 RadialForceComponent 설정
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = ExplosionRadius;
    RadialForceComp->bImpulseVelChange = true; // 순간적인 힘 가함
    RadialForceComp->bIgnoreOwningActor = true; // 자기 자신은 영향 받지 않음
    RadialForceComp->ImpulseStrength = 1500.0f;
}

void AExplosionTrap::ActiveTrap(ACharacter* Target)
{
    // 폭발 효과 발생
    RadialForceComp->FireImpulse();

    // 폭발 범위 내의 캐릭터 감지
    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        ACharacter* Character = Cast<ACharacter>(Actor);
        if (Character)
        {
            // 플레이어가 물리 효과를 받을 수 있도록 설정
            UPrimitiveComponent* CharacterMesh = Cast<UPrimitiveComponent>(Character->GetRootComponent());
            if (CharacterMesh && CharacterMesh->IsSimulatingPhysics())
            {
                FVector Direction = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                CharacterMesh->AddImpulse(Direction * ExplosionForce, NAME_None, true);
            }
        }
    }

    // 폭탄 제거
    Destroy();
}
