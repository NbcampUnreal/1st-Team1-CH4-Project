// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlock/RedEffectBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
//#include "Character/LegoCharacter.h"

ARedEffectBlock::ARedEffectBlock()
{
	EffectDuration = 5.0f;
	BlockLifeTime = 5.0f;
	SpeedBoostValue = 1.2f;
	ImpulseValue = 500.0f;
}

void ARedEffectBlock::ApplyEffect(ACharacter* Target)
{
	if (!Target) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
	if (!MovementComp) return;

	float OriginalSpeed = MovementComp->MaxWalkSpeed;
	float NewSpeed = OriginalSpeed * SpeedBoostValue;

	// �ӵ� ��� + ���޽�
	MovementComp->AddImpulse(PlayerCharacter->GetVelocity().GetSafeNormal() * ImpulseValue);
	MovementComp->MaxWalkSpeed = NewSpeed;

	// ĳ���Ϳ� FX �ٿ��� ����
	//if (ALegoCharacter* LegoChar = Cast<ALegoCharacter>(PlayerCharacter))
	//{
	//	if (LegoChar->SpeedFXTemplate)
	//	{
	//		UNiagaraComponent* TempFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
	//			LegoChar->SpeedFXTemplate,
	//			LegoChar->CameraComp,
	//			NAME_None,
	//			FVector(250.0f, 0.f, -30.f),       // �� �տ��� ����
	//			FRotator(-90.f, 180.f, 0.f),      // ����� ���� ȸ��
	//			EAttachLocation::KeepRelativeOffset,
	//			true
	//		);


	//		if (TempFX)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Niagara Spawned"));

	//			TempFX->SetWorldScale3D(FVector(2.f));
	//			TempFX->SetActive(true, true);
	//			TempFX->Activate(true);

	//			FTimerHandle FXTimerHandle;
	//			GetWorld()->GetTimerManager().SetTimer(FXTimerHandle, [TempFX]()
	//				{
	//					if (TempFX)
	//					{
	//						TempFX->DestroyComponent();
	//						UE_LOG(LogTemp, Warning, TEXT("Niagara Destroyed"));
	//					}
	//				}, EffectDuration, false);
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("Niagara FAILED to spawn!"));
	//		}
	//	}
	//}

	// �ӵ� ����
	TWeakObjectPtr<ACharacter> WeakPlayerCharacter = PlayerCharacter; // ������ ������ ����
	FTimerHandle EffectTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [WeakPlayerCharacter, OriginalSpeed]()
		{
			if (WeakPlayerCharacter.IsValid())
			{
				UCharacterMovementComponent* ResetMovement = WeakPlayerCharacter->GetCharacterMovement();
				if (ResetMovement)
				{
					ResetMovement->MaxWalkSpeed = OriginalSpeed;
				}
			}
		}, EffectDuration, false);
}
