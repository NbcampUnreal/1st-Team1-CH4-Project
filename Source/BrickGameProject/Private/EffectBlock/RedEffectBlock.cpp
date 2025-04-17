// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBlock/RedEffectBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
//#include "Character/LegoCharacter.h"

ARedEffectBlock::ARedEffectBlock()
{
	InitCollsion(true);
	EffectDuration = 5.0f;
	BlockLifeTime = 5.0f;
	SpeedBoostValue = 2.0f;
	LaunchPower = 1500.0f;
}

void ARedEffectBlock::ApplyEffect(ACharacter* Target)
{
	if (!Target) return;

	static TMap<ACharacter*, FTimerHandle> ActiveEffects; // �Լ� �ȿ� ������, static�̶� ���������� ������!

	

	ACharacter* PlayerCharacter = Cast<ACharacter>(Target);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();
	if (!MovementComp) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlap First Speed : %f"), MovementComp->MaxWalkSpeed));
	if (ActiveEffects.Contains(Target))
	{
		// ���� ȿ���� ������� �ǵ�����, Ÿ�̸� �ʱ�ȭ
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

	// ������ �о��ֱ�
	FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() * LaunchPower;
	PlayerCharacter->LaunchCharacter(LaunchDirection, true, true);
	if (BoostSound && PlayerCharacter)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BoostSound, PlayerCharacter->GetActorLocation());
	}

	// �ӵ� ����
	MovementComp->MaxWalkSpeed = NewSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Change Speed : %f"), MovementComp->MaxWalkSpeed));

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
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Timer - Reset : %f"), ResetMovement->MaxWalkSpeed));
				}
			}
			// ActiveEffects���� ����
			ActiveEffects.Remove(WeakPlayerCharacter.Get());

		}, EffectDuration, false);

	ActiveEffects.Add(Target, EffectTimerHandle);
}
