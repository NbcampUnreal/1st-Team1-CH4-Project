// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/FloorButton.h"
#include "Components/CapsuleComponent.h"
#include "CooperatingDevice/FloorButtonSet.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFloorButton::AFloorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Root Component(Scene Component)
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComp);

	// Collision Component
	/*CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleComp->SetupAttachment(SceneComp);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::OnOverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::OnOverlapEnd);*/

	// Static Mesh Component
	StaticMeshBaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	StaticMeshBaseComp->SetupAttachment(SceneComp);

	StaticMeshButtonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	StaticMeshButtonComp->SetupAttachment(StaticMeshBaseComp);
	StaticMeshButtonComp->SetGenerateOverlapEvents(true);
	StaticMeshButtonComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 혹은 QueryAndPhysics
	StaticMeshButtonComp->SetCollisionObjectType(ECC_WorldDynamic);
	StaticMeshButtonComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshButtonComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StaticMeshButtonComp->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::OnOverlapBegin);
	StaticMeshButtonComp->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::OnOverlapEnd);
}

void AFloorButton::BeginPlay()
{
	Super::BeginPlay();

	InitialButtonLocation = StaticMeshButtonComp->GetRelativeLocation();
	PressedButtonLocation = InitialButtonLocation + FVector(0.f, 0.f, PressDepth);

	SetButtonMaterial(DefaultMaterial);
}

void AFloorButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		OverlappingPlayers.AddUnique(Player);
		UpdateButtonState();
	}
}

void AFloorButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		OverlappingPlayers.Remove(Player);
		UpdateButtonState();
	}
}

void AFloorButton::UpdateButtonState()
{
	if (bLocked) return;

	bIsPressed = OverlappingPlayers.Num() > 0;

	if (bIsPressed)
	{
		PlayPressAnimation();
	}
	else
	{
		PlayReleaseAnimation();
	}
}

// 버튼 눌리는 애니메이션 재생 및 색상 전환
void AFloorButton::PlayPressAnimation()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(
		StaticMeshButtonComp,
		PressedButtonLocation,
		StaticMeshButtonComp->GetRelativeRotation(),
		false, false,
		MoveDuration,
		false,
		EMoveComponentAction::Move,
		LatentInfo
	);

	SetButtonMaterial(PressedMaterial);
}

// 버튼 올라오는 애니메이션 재생 및 색상 전환
void AFloorButton::PlayReleaseAnimation()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(
		StaticMeshButtonComp,
		InitialButtonLocation,
		StaticMeshButtonComp->GetRelativeRotation(),
		false, false,
		MoveDuration,
		false,
		EMoveComponentAction::Move,
		LatentInfo
	);

	SetButtonMaterial(DefaultMaterial);
}

void AFloorButton::SetButtonMaterial(UMaterialInterface* NewMaterial)
{
	if (StaticMeshButtonComp)
	{
		if (NewMaterial)
		{
			StaticMeshButtonComp->SetMaterial(0, NewMaterial);
		}
	}
}

