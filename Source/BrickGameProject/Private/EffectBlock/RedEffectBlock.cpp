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
	LaunchPower = 1500.0f;
}

void ARedEffectBlock::ApplyEffect(ACharacter* Target)
{
	if (!Target) return;

	static TMap<ACharacter*, FTimerHandle> ActiveEffects; // 함수 안에 있지만, static이라 전역적으로 유지됨!

	

	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
	if (!MovementComp) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlap First Speed : %f"), MovementComp->MaxWalkSpeed));
	if (ActiveEffects.Contains(Target))
	{
		// 기존 효과를 원래대로 되돌리고, 타이머 초기화
		if (MovementComp)
		{
			MovementComp->MaxWalkSpeed /= SpeedBoostValue;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Contain - Reset : %f"), MovementComp->MaxWalkSpeed));
		}
		GetWorld()->GetTimerManager().ClearTimer(ActiveEffects[Target]);
		ActiveEffects.Remove(Target);
	}

	float OriginalSpeed = MovementComp->MaxWalkSpeed;
	float NewSpeed = OriginalSpeed * SpeedBoostValue;

	// 앞으로 밀어주기
	FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() * LaunchPower;
	PlayerCharacter->LaunchCharacter(LaunchDirection, true, true);

	// 속도 변경
	MovementComp->MaxWalkSpeed = NewSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Change Speed : %f"), MovementComp->MaxWalkSpeed));

	// 캐릭터에 FX 붙여서 실행
	//if (ALegoCharacter* LegoChar = Cast<ALegoCharacter>(PlayerCharacter))
	//{
	//	if (LegoChar->SpeedFXTemplate)
	//	{
	//		UNiagaraComponent* TempFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
	//			LegoChar->SpeedFXTemplate,
	//			LegoChar->CameraComp,
	//			NAME_None,
	//			FVector(250.0f, 0.f, -30.f),       // 더 앞에서 시작
	//			FRotator(-90.f, 180.f, 0.f),      // 위→앞 방향 회전
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

	// 속도 원복
	TWeakObjectPtr<ACharacter> WeakPlayerCharacter = PlayerCharacter; // 안전한 참조를 위함
	FTimerHandle EffectTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [WeakPlayerCharacter, OriginalSpeed]()
		{
			if (WeakPlayerCharacter.IsValid())
			{
				UCharacterMovementComponent* ResetMovement = WeakPlayerCharacter->GetCharacterMovement();
				if (ResetMovement)
				{
					ResetMovement->MaxWalkSpeed = OriginalSpeed;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Timer - Reset : %f"), ResetMovement->MaxWalkSpeed));
				}
			}
			// ActiveEffects에서 제거
			ActiveEffects.Remove(WeakPlayerCharacter.Get());

		}, EffectDuration, false);

	ActiveEffects.Add(Target, EffectTimerHandle);
}
