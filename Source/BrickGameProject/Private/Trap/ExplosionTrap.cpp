// Fill out your copyright notice in the Description page of Project Settings.

#include "Trap/ExplosionTrap.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BrickCharacter.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"

AExplosionTrap::AExplosionTrap()
{
    // Collision 초기화
    InitCollision(true, false);

    // 폭발 물리 힘을 주기 위한 RadialForceComponent 생성
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = ExplosionRadius;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->bIgnoreOwningActor = true;
    RadialForceComp->ImpulseStrength = ExplosionForce;
}

void AExplosionTrap::ActiveTrap(ACharacter* Target)
{
    // 메시 숨기고 충돌 제거
    if (StaticMeshComp)
    {
        StaticMeshComp->SetVisibility(false);
        StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    InitCollision(false, false);

    // 폭발 비주얼 이펙트 재생
    if (ExplosionNiagaraSystem)
    {
        UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionNiagaraSystem,
            GetActorLocation(),
            GetActorRotation()
        );

        if (NiagaraComponent)
        {
            NiagaraComponent->OnSystemFinished.AddDynamic(this, &AExplosionTrap::OnExplosionFinished);
        }
    }


    RadialForceComp->FireImpulse();


    if (Target && StarEffectNiagaraSystem)
    {
        UNiagaraComponent* StarEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
            StarEffectNiagaraSystem,
            Target->GetMesh(),
            FName("Head_bone"),
            FVector(0.f, 0.f, 20.f),
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            false 
        );

        if (StarEffect && GetWorld())
        {
            FTimerHandle StarDestroyHandle;
            GetWorld()->GetTimerManager().SetTimer(
                StarDestroyHandle,
                FTimerDelegate::CreateLambda([StarEffect]()
                    {
                        StarEffect->DestroyComponent();
                    }),
                8.0f, false
            );
        }

    }
    if (Target && Target->IsPlayerControlled())
    {
        UUserWidget* Widget = nullptr;

        Widget = Cast<UUserWidget>(Target->FindComponentByClass<UWidgetComponent>());

        if (!Widget)
        {
            
            ABrickCharacter* MyChar = Cast<ABrickCharacter>(Target);
            if (MyChar && MyChar->DamageInstance)
            {
                Widget = MyChar->DamageInstance;
            }
        }

        if (Widget)
        {
            UFunction* FlashFunc = Widget->FindFunction(TEXT("PlayFlash"));
            if (FlashFunc)
            {
                Widget->ProcessEvent(FlashFunc, nullptr);
            }
        }
    }
}

void AExplosionTrap::OnExplosionFinished(UNiagaraComponent* FinishedComponent)
{
    // 트랩 제거
    Destroy();
}
