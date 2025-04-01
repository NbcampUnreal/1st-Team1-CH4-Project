#include "EffectBlocks/RedBlock.h"
#include "Character/LegoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ARedBlock::ARedBlock()
{
	EffectDuration = 5.0f;
	BlockLifeTime = 5.0f;
	SpeedBoostValue = 1.2f;
	ImpulseValue = 500.0f;
}

void ARedBlock::ApplyEffect(ACharacter* Target)
{
	if (!HasAuthority()) return;

	if (!Target) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
	if (!MovementComp) return;

	float OriginalSpeed = MovementComp->MaxWalkSpeed;
	float NewSpeed = OriginalSpeed * SpeedBoostValue;

	// 🚀 속도 상승 + 임펄스
	MovementComp->AddImpulse(PlayerCharacter->GetVelocity().GetSafeNormal() * ImpulseValue);
	MovementComp->MaxWalkSpeed = NewSpeed;

	// ✅ 캐릭터에 FX 붙여서 실행
	if (ALegoCharacter* LegoChar = Cast<ALegoCharacter>(PlayerCharacter))
	{
		if (LegoChar->SpeedFXTemplate)
		{
			UNiagaraComponent* TempFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
				LegoChar->SpeedFXTemplate,
				LegoChar->CameraComp,
				NAME_None,
				FVector(250.0f, 0.f, -30.f),       // ✅ 더 앞에서 시작
				FRotator(-90.f, 180.f, 0.f),      // ✅ 위→앞 방향 회전
				EAttachLocation::KeepRelativeOffset,
				true
			);


			if (TempFX)
			{
				UE_LOG(LogTemp, Warning, TEXT("🚀 Niagara Spawned"));

				TempFX->SetWorldScale3D(FVector(2.f));
				TempFX->SetActive(true, true);
				TempFX->Activate(true);

				FTimerHandle FXTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(FXTimerHandle, [TempFX]()
					{
						if (TempFX)
						{
							TempFX->DestroyComponent();
							UE_LOG(LogTemp, Warning, TEXT("🧹 Niagara Destroyed"));
						}
					}, EffectDuration, false);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("❌ Niagara FAILED to spawn!"));
			}
		}
	}

	// ⏱️ 속도 원복
	FTimerHandle EffectTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [PlayerCharacter, OriginalSpeed]()
		{
			if (PlayerCharacter)
			{
				UCharacterMovementComponent* ResetMovement = PlayerCharacter->GetCharacterMovement();
				if (ResetMovement)
				{
					ResetMovement->MaxWalkSpeed = OriginalSpeed;
				}
			}
		}, EffectDuration, false);
}
