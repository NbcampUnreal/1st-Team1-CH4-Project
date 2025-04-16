// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/BrickCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABrickCharacter::ABrickCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 0.5f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bDoCollisionTest = true;
	SpringArmComp->ProbeSize = 26.0f;
	SpringArmComp->ProbeChannel = ECC_Camera;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 10.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	PreviewPivotToBottom = 0.0f;


	PreviewBlocks.SetNum(2);
}


// Called when the game starts or when spawned
void ABrickCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ABrickCharacter::OnHitByObstacle);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

// Called every frame
void ABrickCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviewBlocks.IsValidIndex(SelectedBlockIndex) && PreviewBlocks[SelectedBlockIndex])
	{
		UpdatePreviewBlock();
	}

}

// Called to bind functionality to input
void ABrickCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ABrickGamePlayerController* PlayerController = Cast<ABrickGamePlayerController>(GetController()))
		{

			EnhancedInput->BindAction(PlayerController->GetMoveAction(), ETriggerEvent::Triggered, this, &ABrickCharacter::Move);
			EnhancedInput->BindAction(PlayerController->GetJumpAction(), ETriggerEvent::Triggered, this, &ABrickCharacter::StartJump);
			EnhancedInput->BindAction(PlayerController->GetJumpAction(), ETriggerEvent::Completed, this, &ABrickCharacter::StopJump);
			EnhancedInput->BindAction(PlayerController->GetLookAction(), ETriggerEvent::Triggered, this, &ABrickCharacter::Look);
			EnhancedInput->BindAction(PlayerController->GetBlock1Action(), ETriggerEvent::Triggered, this, &ABrickCharacter::SelectBlock1);
			EnhancedInput->BindAction(PlayerController->GetBlock2Action(), ETriggerEvent::Triggered, this, &ABrickCharacter::SelectBlock2);
			EnhancedInput->BindAction(PlayerController->GetRotatePreviewBlockAction(), ETriggerEvent::Triggered, this, &ABrickCharacter::RotatePreviewBlock);
			EnhancedInput->BindAction(PlayerController->GetDeleteBlockAction(), ETriggerEvent::Started, this, &ABrickCharacter::DeleteBlock);
			EnhancedInput->BindAction(PlayerController->GetLeftClickAction(), ETriggerEvent::Started, this, &ABrickCharacter::OnLeftClick);
			EnhancedInput->BindAction(PlayerController->GetHoldFAction(), ETriggerEvent::Started, this, &ABrickCharacter::PlayFKeyAnimationStart);
			EnhancedInput->BindAction(PlayerController->GetHoldFAction(), ETriggerEvent::Completed, this, &ABrickCharacter::PlayFKeyAnimationStop);
		}
	}

}


void ABrickCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	FRotator ControlRot = Controller->GetControlRotation(); // Get current control rotation(usually from the camera)
	FRotator YawRotation(0, ControlRot.Yaw, 0); // Remove pitch, roll(keep only yaw for horizontal movement)

	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Forward direction relative to the camera's yaw
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Right direction relative to the camera's yaw

	if (!FMath::IsNearlyZero(MoveInput.X)) AddMovementInput(Forward, MoveInput.X);
	if (!FMath::IsNearlyZero(MoveInput.Y)) AddMovementInput(Right, MoveInput.Y);
}

void ABrickCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();

		if (JumpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
		}
	}
}

void ABrickCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) StopJumping();
}

void ABrickCharacter::Look(const FInputActionValue& value)
{

	if (!bCanTurn) return; // 🔒 마우스 회전 차단

	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABrickCharacter::StartPlacingBlock(const FInputActionValue& Value)
{
	if (!PreviewBlockClasses.IsValidIndex(SelectedBlockIndex)) return;

	if (PreviewBlocks.IsValidIndex(SelectedBlockIndex) && PreviewBlocks[SelectedBlockIndex])
	{
		PreviewBlocks[SelectedBlockIndex]->Destroy();
		PreviewBlocks[SelectedBlockIndex] = nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(PreviewBlockClasses[SelectedBlockIndex], FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (Spawned)
	{
		TArray<UPrimitiveComponent*> PrimComponents;
		Spawned->GetComponents<UPrimitiveComponent>(PrimComponents);
		for (auto Comp : PrimComponents)
		{
			Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Comp->SetSimulatePhysics(false);
		}
		if (SelectedBlockIndex == 1)
		{
			FRotator Rot = Spawned->GetActorRotation();
			Rot.Yaw += 180.f;
			Spawned->SetActorRotation(Rot);
		}

		PreviewBlocks[SelectedBlockIndex] = Spawned;
	}

}


void ABrickCharacter::SelectBlock1(const FInputActionValue& value)
{
	if (!bCanUseSkill) return;
	if (value.Get<bool>())
	{
		SelectedBlockIndex = 0;

		for (int i = 0; i < PreviewBlocks.Num(); i++)
		{
			if (i != SelectedBlockIndex && PreviewBlocks[i])
			{
				PreviewBlocks[i]->Destroy();
				PreviewBlocks[i] = nullptr;
			}
		}

		StartPlacingBlock(FInputActionValue());
	}
}


void ABrickCharacter::SelectBlock2(const FInputActionValue& value)
{
	if (!bCanUseSkill) return;
	if (value.Get<bool>())
	{
		SelectedBlockIndex = 1;

		for (int i = 0; i < PreviewBlocks.Num(); i++)
		{
			if (i != SelectedBlockIndex && PreviewBlocks[i])
			{
				PreviewBlocks[i]->Destroy();
				PreviewBlocks[i] = nullptr;
			}
		}
		StartPlacingBlock(FInputActionValue());
	}
}

void ABrickCharacter::RotatePreviewBlock(const FInputActionValue& Value)
{
	if (!PreviewBlocks.IsValidIndex(SelectedBlockIndex)) return;

	AActor* CurrentPreview = PreviewBlocks[SelectedBlockIndex];
	if (!CurrentPreview) return;

	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue)) return;

	FRotator CurrentRotation = CurrentPreview->GetActorRotation();
	CurrentRotation.Yaw += ScrollValue * 15.0f;
	CurrentPreview->SetActorRotation(CurrentRotation);
}

void ABrickCharacter::DeleteBlock(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FHitResult HitResult;
	if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			const FString Name = HitActor->GetName();
			if (Name.Contains(TEXT("Block1")) || Name.Contains(TEXT("Block2")) || Name.Contains(TEXT("Block3")))
			{
				HitActor->Destroy();
			}
		}
	}

}

void ABrickCharacter::OnLeftClick(const FInputActionValue& Value)
{
	if (!bCanUseSkill) return;
	if (!PreviewBlocks.IsValidIndex(SelectedBlockIndex)) return;

	AActor* CurrentPreview = PreviewBlocks[SelectedBlockIndex];
	if (!CurrentPreview || !BlockClasses.IsValidIndex(SelectedBlockIndex)) return;

	FVector Origin, Extent;
	CurrentPreview->GetActorBounds(false, Origin, Extent);

	FVector SpawnLocation = Origin - FVector(0, 0, Extent.Z);
	FRotator SpawnRotation = CurrentPreview->GetActorRotation();

	ServerPlaceBlock(SpawnLocation, SpawnRotation);

	CurrentPreview->Destroy();
	PreviewBlocks[SelectedBlockIndex] = nullptr;

}


void ABrickCharacter::MulticastPlayVictoryMontage_Implementation()
{
	if (VictoryMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(VictoryMontage);
	}
}

void ABrickCharacter::MulticastPlayDefeatMontage_Implementation()
{
	if (DefeatMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (!AnimInst->Montage_IsPlaying(DefeatMontage))
		{
			AnimInst->Montage_Play(DefeatMontage, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("▶▶ Multicast DefeatMontage 재생"));
		}
	}
}



void ABrickCharacter::UpdatePreviewBlock()
{
	if (!PreviewBlocks.IsValidIndex(SelectedBlockIndex)) return;

	AActor* CurrentPreview = PreviewBlocks[SelectedBlockIndex];
	if (!CurrentPreview) return;

	FHitResult Hit;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		if (Hit.bBlockingHit)
		{
			FVector Origin, Extent;
			CurrentPreview->GetActorBounds(true, Origin, Extent);
			FVector Adjusted = Hit.ImpactPoint;
			Adjusted.Z += Extent.Z;
			CurrentPreview->SetActorLocation(Adjusted);
		}
	}
}




void ABrickCharacter::PlayFKeyAnimationStart(const FInputActionValue& Value)
{
	if (FKeyMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (!AnimInst->Montage_IsPlaying(FKeyMontage))
		{
			AnimInst->Montage_Play(FKeyMontage, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("F key held - Montage Playing"));
		}
	}
}

void ABrickCharacter::PlayFKeyAnimationStop(const FInputActionValue& Value)
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
		UE_LOG(LogTemp, Warning, TEXT("F key released - Montage Stopped"));
	}
}

void ABrickCharacter::SetCanUseSkill(bool NewCanUseSkill)
{
	bCanUseSkill = NewCanUseSkill;
}

void ABrickCharacter::AttachCrown()
{
	if (!CrownStaticMesh) return;

	UStaticMeshComponent* Crown = NewObject<UStaticMeshComponent>(this);
	Crown->RegisterComponent();
	Crown->SetStaticMesh(CrownStaticMesh);
	Crown->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Head_bone"));
	Crown->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	Crown->SetRelativeScale3D(FVector(0.6f));
};


void ABrickCharacter::PlayVictoryMontage()
{
	if (VictoryMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(VictoryMontage);
	}
}

void ABrickCharacter::PlayDefeatMontage()
{
	if (DefeatMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

		SetMovementEnabled(false);
		bCanTurn = false;

		if (!AnimInst->Montage_IsPlaying(DefeatMontage))
		{
			AnimInst->Montage_Play(DefeatMontage, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("▶▶ DefeatMontage 재생 시작"));
		}
	}
}



void ABrickCharacter::SetMovementEnabled(bool bEnabled)
{
	bCanMove = bEnabled;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		if (bEnabled)
		{
			MoveComp->SetMovementMode(MOVE_Walking);
		}
		else
		{
			MoveComp->DisableMovement(); 
		}
	}
}

bool ABrickCharacter::CanBeMoved() const
{
	return bCanMove;
}

void ABrickCharacter::MulticastFixMeshRotation_Implementation(FRotator NewRotation)
{
	if (GetMesh())
	{
		GetMesh()->SetRelativeRotation(NewRotation);
	}
}

void ABrickCharacter::MulticastApplyFinalPose_Implementation(FRotator ActorRot, FRotator MeshRot)
{
	UE_LOG(LogTemp, Warning, TEXT("MulticastApplyFinalPose CALLED on %s. Rot: %s"), *GetName(), *ActorRot.ToString());

	SetActorRotation(ActorRot);
	if (GetMesh())
	{
		GetMesh()->SetRelativeRotation(MeshRot);
	}
}
void ABrickCharacter::ServerPlaceBlock_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (BlockClasses.IsValidIndex(SelectedBlockIndex))
	{
		AActor* SpawnedBlock = GetWorld()->SpawnActor<AActor>(
			BlockClasses[SelectedBlockIndex], SpawnLocation, SpawnRotation
		);

		if (ClickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
		}
	}
}
void ABrickCharacter::ApplyKnockback(const FVector& Direction, float Strength)
{
	if (!Controller || !GetCharacterMovement()) return;

	FVector LaunchVelocity = Direction.GetSafeNormal() * Strength;
	LaunchCharacter(LaunchVelocity, true, true);
	SetMovementEnabled(false);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABrickCharacter::EnableMovementAfterKnockback, 1.0f, false);
}

void ABrickCharacter::EnableMovementAfterKnockback()
{
	SetMovementEnabled(true);
}


void ABrickCharacter::OnHitByObstacle(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Obstacle")))
	{
		FVector KnockbackDir = Hit.ImpactNormal * -1.0f;
		KnockbackDir.Z += 0.4f; 
		ApplyKnockback(KnockbackDir, 1500.f);
	}
}


