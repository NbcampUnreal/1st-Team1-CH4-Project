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
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 0.5f;

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

// Called when the game starts or when spawned
void ABrickCharacter::BeginPlay()
{
	Super::BeginPlay();


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

	if (PreviewBlock)
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
			EnhancedInput->BindAction(PlayerController->GetBlock3Action(), ETriggerEvent::Triggered, this, &ABrickCharacter::SelectBlock3);
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
	FRotator ControlRot = Controller->GetControlRotation();
	FRotator YawRotation(0, ControlRot.Yaw, 0); 

	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); 

	if (!FMath::IsNearlyZero(MoveInput.X)) AddMovementInput(Forward, MoveInput.X);
	if (!FMath::IsNearlyZero(MoveInput.Y)) AddMovementInput(Right, MoveInput.Y);
}

void ABrickCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>()) Jump();

}

void ABrickCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) StopJumping();
}

void ABrickCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABrickCharacter::StartPlacingBlock(const FInputActionValue& Value)
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

void ABrickCharacter::SelectBlock1(const FInputActionValue& value)
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

void ABrickCharacter::SelectBlock2(const FInputActionValue& value)
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

void ABrickCharacter::SelectBlock3(const FInputActionValue& value)
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

void ABrickCharacter::RotatePreviewBlock(const FInputActionValue& Value)
{
	if (!PreviewBlock) return;
	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue)) return;
	FRotator CurrentRotation = PreviewBlock->GetActorRotation();
	CurrentRotation.Yaw += ScrollValue * 15.0f;
	PreviewBlock->SetActorRotation(CurrentRotation);
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



void ABrickCharacter::UpdatePreviewBlock()
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
		GetMesh()->GetAnimInstance()->Montage_Play(DefeatMontage);
	}
}
