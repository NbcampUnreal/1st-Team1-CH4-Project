#include "Character/LegoCharacter.h"
#include "Character/LegoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALegoCharacter::ALegoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	PreviewBlock = nullptr;
	PreviewPivotToBottom = 0.0f;
}

void ALegoCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsPlacingMode = false;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	if (SpeedFXTemplate && CameraComp)
	{
		SpeedFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SpeedFXTemplate,
			CameraComp,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		SpeedFX->Deactivate(); 
	}
}

void ALegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePreviewBlock();

	if (CameraComp)
	{
		const float Speed = GetVelocity().Size();
		const float TargetFOV = FMath::GetMappedRangeValueClamped(
			FVector2D(300.f, 600.f),   
			FVector2D(90.f, 95.f),      
			Speed
		);


		const float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, 5.f);
		CameraComp->SetFieldOfView(NewFOV);
	}
}


void ALegoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ALegoPlayerController* PlayerController = Cast<ALegoPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ALegoCharacter::Move);
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ALegoCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ALegoCharacter::StopJump);
			}
			if (PlayerController->LookAction)
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ALegoCharacter::Look);
			if (PlayerController->StartPlaceBlockAction)
				EnhancedInput->BindAction(PlayerController->StartPlaceBlockAction, ETriggerEvent::Started, this, &ALegoCharacter::StartPlacingBlock);
			if (PlayerController->ConfirmPlaceBlockAction)
				EnhancedInput->BindAction(PlayerController->ConfirmPlaceBlockAction, ETriggerEvent::Started, this, &ALegoCharacter::ConfirmPlacingBlock);
			if (PlayerController->Block1Action)
				EnhancedInput->BindAction(PlayerController->Block1Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock1);
			if (PlayerController->Block2Action)
				EnhancedInput->BindAction(PlayerController->Block2Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock2);
			if (PlayerController->Block3Action)
				EnhancedInput->BindAction(PlayerController->Block3Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock3);
			if (PlayerController->RotatePreviewBlockAction)
				EnhancedInput->BindAction(PlayerController->RotatePreviewBlockAction, ETriggerEvent::Triggered, this, &ALegoCharacter::RotatePreviewBlock);
			if (PlayerController->FKeyAction)
				EnhancedInput->BindAction(PlayerController->FKeyAction, ETriggerEvent::Triggered, this, &ALegoCharacter::PlayFKeyAnimation);
		}
	}
}

void ALegoCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveInput.X)) AddMovementInput(GetActorForwardVector(), MoveInput.X);
	if (!FMath::IsNearlyZero(MoveInput.Y)) AddMovementInput(GetActorRightVector(), MoveInput.Y);
}

void ALegoCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>()) Jump();
}

void ALegoCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) StopJumping();
}

void ALegoCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ALegoCharacter::RotatePreviewBlock(const FInputActionValue& Value)
{
	if (!PreviewBlock) return;
	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue)) return;
	FRotator CurrentRotation = PreviewBlock->GetActorRotation();
	CurrentRotation.Yaw += ScrollValue * 15.0f;
	PreviewBlock->SetActorRotation(CurrentRotation);
}

void ALegoCharacter::StartPlacingBlock(const FInputActionValue& value)
{
	if (!PreviewBlock && PreviewBlockClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PreviewBlock = GetWorld()->SpawnActor<AActor>(PreviewBlockClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		if (PreviewBlock)
		{
			TArray<UPrimitiveComponent*> PrimComponents;
			PreviewBlock->GetComponents<UPrimitiveComponent>(PrimComponents);
			for (UPrimitiveComponent* PrimComp : PrimComponents)
			{
				PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PrimComp->SetSimulatePhysics(false);
			}
		}
	}
}

void ALegoCharacter::UpdatePreviewBlock()
{
	if (!PreviewBlock) return;

	FHitResult Hit;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		if (Hit.bBlockingHit)
		{
			FVector Origin, Extent;
			PreviewBlock->GetActorBounds(true, Origin, Extent);

			FVector Adjusted = Hit.ImpactPoint;
			Adjusted.Z += Extent.Z; // 정확히 바닥 위에 배치
			PreviewBlock->SetActorLocation(Adjusted);
		}
	}
}

void ALegoCharacter::ConfirmPlacingBlock(const FInputActionValue& value)
{
	if (PreviewBlock && BlockClasses.IsValidIndex(SelectedBlockIndex))
	{
		FVector Origin, Extent;
		PreviewBlock->GetActorBounds(false, Origin, Extent);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(PreviewBlock);

		TArray<UPrimitiveComponent*> PrimComponents;
		PreviewBlock->GetComponents<UPrimitiveComponent>(PrimComponents);
		for (UPrimitiveComponent* PrimComp : PrimComponents)
		{
			Params.AddIgnoredComponent(PrimComp);
		}

		bool bOverlaps = GetWorld()->OverlapAnyTestByChannel(
			Origin, FQuat::Identity, ECC_WorldStatic,
			FCollisionShape::MakeBox(Extent), Params
		);

		if (bOverlaps)
		{
			UE_LOG(LogTemp, Warning, TEXT("🚫 겹쳐서 블록을 놓을 수 없습니다!"));
			return;
		}

		// ✅ 진짜 바닥 위치 계산
		FVector SpawnLocation = Origin - FVector(0, 0, Extent.Z);
		FRotator SpawnRotation = PreviewBlock->GetActorRotation();

		GetWorld()->SpawnActor<AActor>(BlockClasses[SelectedBlockIndex], SpawnLocation, SpawnRotation);

		PreviewBlock->Destroy();
		PreviewBlock = nullptr;
	}
}


void ALegoCharacter::SelectBlock1(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		SelectedBlockIndex = 0;
		if (PreviewBlock)
		{
			PreviewBlock->Destroy();
			PreviewBlock = nullptr;
		}
	}
}

void ALegoCharacter::SelectBlock2(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		SelectedBlockIndex = 1;
		if (PreviewBlock)
		{
			PreviewBlock->Destroy();
			PreviewBlock = nullptr;
		}
	}
}

void ALegoCharacter::SelectBlock3(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		SelectedBlockIndex = 2;
		if (PreviewBlock)
		{
			PreviewBlock->Destroy();
			PreviewBlock = nullptr;
		}
	}
}

void ALegoCharacter::PlayFKeyAnimation(const FInputActionValue& Value)
{
	if (FKeyMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(FKeyMontage);
		UE_LOG(LogTemp, Warning, TEXT("F key pressed - Montage Played"));
	}
}

void ALegoCharacter::TogglePlacementMode()
{
	bIsPlacingMode = !bIsPlacingMode;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		if (bIsPlacingMode)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}
