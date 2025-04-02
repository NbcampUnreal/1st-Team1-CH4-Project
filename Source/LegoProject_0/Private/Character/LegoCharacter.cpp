#include "Character/LegoCharacter.h"
#include "Character/LegoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

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

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void ALegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviewBlock)
	{
		UpdatePreviewBlock();
	}
}

void ALegoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ALegoPlayerController* PlayerController = Cast<ALegoPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ALegoCharacter::Move);
			EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ALegoCharacter::StartJump);
			EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ALegoCharacter::StopJump);
			EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ALegoCharacter::Look);
			EnhancedInput->BindAction(PlayerController->Block1Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock1);
			EnhancedInput->BindAction(PlayerController->Block2Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock2);
			EnhancedInput->BindAction(PlayerController->Block3Action, ETriggerEvent::Triggered, this, &ALegoCharacter::SelectBlock3);
			EnhancedInput->BindAction(PlayerController->RotatePreviewBlockAction, ETriggerEvent::Triggered, this, &ALegoCharacter::RotatePreviewBlock);
			EnhancedInput->BindAction(PlayerController->DeleteBlockAction, ETriggerEvent::Started, this, &ALegoCharacter::DeleteBlock);
			EnhancedInput->BindAction(PlayerController->LeftClickAction, ETriggerEvent::Started, this, &ALegoCharacter::OnLeftClick);
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
			Adjusted.Z += Extent.Z;
			PreviewBlock->SetActorLocation(Adjusted);
		}
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
		FInputActionValue Dummy;
		StartPlacingBlock(Dummy);
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
		FInputActionValue Dummy;
		StartPlacingBlock(Dummy);
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
		FInputActionValue Dummy;
		StartPlacingBlock(Dummy);
	}
}

void ALegoCharacter::OnLeftClick(const FInputActionValue& Value)
{
	if (!PreviewBlock || !BlockClasses.IsValidIndex(SelectedBlockIndex)) return;

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
		return;
	}

	FVector SpawnLocation = Origin - FVector(0, 0, Extent.Z);
	FRotator SpawnRotation = PreviewBlock->GetActorRotation();

	GetWorld()->SpawnActor<AActor>(BlockClasses[SelectedBlockIndex], SpawnLocation, SpawnRotation);

	PreviewBlock->Destroy();
	PreviewBlock = nullptr;
}

void ALegoCharacter::DeleteBlock(const FInputActionValue& Value)
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
void ALegoCharacter::PlayFKeyAnimation(const FInputActionValue& Value)
{
	if (FKeyMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(FKeyMontage);
		UE_LOG(LogTemp, Warning, TEXT("F key pressed - Montage Played"));
	}
}