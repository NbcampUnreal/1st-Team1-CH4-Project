// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/BricCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Character/BricPlayerController.h"


// Sets default values
ABricCharacter::ABricCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void ABricCharacter::BeginPlay()
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
void ABricCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviewBlock)
	{
		UpdatePreviewBlock();
	}


}

// Called to bind functionality to input
void ABricCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ABricPlayerController* PlayerController = Cast<ABricPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ABricCharacter::Move);
			EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ABricCharacter::StartJump);
			EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ABricCharacter::StopJump);
			EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ABricCharacter::Look);
			EnhancedInput->BindAction(PlayerController->Block1Action, ETriggerEvent::Triggered, this, &ABricCharacter::SelectBlock1);
			EnhancedInput->BindAction(PlayerController->Block2Action, ETriggerEvent::Triggered, this, &ABricCharacter::SelectBlock2);
			EnhancedInput->BindAction(PlayerController->Block3Action, ETriggerEvent::Triggered, this, &ABricCharacter::SelectBlock3);
			EnhancedInput->BindAction(PlayerController->RotatePreviewBlockAction, ETriggerEvent::Triggered, this, &ABricCharacter::RotatePreviewBlock);
			EnhancedInput->BindAction(PlayerController->DeleteBlockAction, ETriggerEvent::Started, this, &ABricCharacter::DeleteBlock);
			EnhancedInput->BindAction(PlayerController->LeftClickAction, ETriggerEvent::Started, this, &ABricCharacter::OnLeftClick);
			EnhancedInput->BindAction(PlayerController->HoldFAction, ETriggerEvent::Started, this, &ABricCharacter::PlayFKeyAnimationStart);
			EnhancedInput->BindAction(PlayerController->HoldFAction, ETriggerEvent::Completed, this, &ABricCharacter::PlayFKeyAnimationStop);

		}
	}


}

void ABricCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveInput.X)) AddMovementInput(GetActorForwardVector(), MoveInput.X);
	if (!FMath::IsNearlyZero(MoveInput.Y)) AddMovementInput(GetActorRightVector(), MoveInput.Y);

}

void ABricCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>()) Jump();

}

void ABricCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) StopJumping();
}

void ABricCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABricCharacter::StartPlacingBlock(const FInputActionValue& Value)
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

void ABricCharacter::SelectBlock1(const FInputActionValue& value)
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

void ABricCharacter::SelectBlock2(const FInputActionValue& value)
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

void ABricCharacter::SelectBlock3(const FInputActionValue& value)
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

void ABricCharacter::RotatePreviewBlock(const FInputActionValue& Value)
{
	if (!PreviewBlock) return;
	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue)) return;
	FRotator CurrentRotation = PreviewBlock->GetActorRotation();
	CurrentRotation.Yaw += ScrollValue * 15.0f;
	PreviewBlock->SetActorRotation(CurrentRotation);
}

void ABricCharacter::DeleteBlock(const FInputActionValue& Value)
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

void ABricCharacter::OnLeftClick(const FInputActionValue& Value)
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



void ABricCharacter::UpdatePreviewBlock()
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


void ABricCharacter::PlayFKeyAnimationStart(const FInputActionValue& Value)
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

void ABricCharacter::PlayFKeyAnimationStop(const FInputActionValue& Value)
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
		UE_LOG(LogTemp, Warning, TEXT("F key released - Montage Stopped"));
	}
}
