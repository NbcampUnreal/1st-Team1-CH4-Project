#include "InGame/KillVolume.h"
#include "Character/BrickCharacter.h"
#include "Network_Structure/BrickGamePlayerState.h"
// Components
#include "Components/BoxComponent.h"


AKillVolume::AKillVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBegin);
}

void AKillVolume::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ABrickCharacter* Character = Cast<ABrickCharacter>(OtherActor);
	ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(Character->GetPlayerState());
	if ( PS)
	{
		if (Character && Character->HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Saved checkpoint: %s"), *PS->GetCurrentCheckPoint().ToString());
			Character->TeleportTo(PS->GetCurrentCheckPoint(), Character->GetActorRotation());

			bool bSuccess = Character->TeleportTo(PS->GetCurrentCheckPoint(), Character->GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("Teleport success: %s"), bSuccess ? TEXT("true") : TEXT("false"));
		}
	}
}

