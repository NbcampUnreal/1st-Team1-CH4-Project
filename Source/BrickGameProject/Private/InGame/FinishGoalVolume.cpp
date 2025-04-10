#include "InGame/FinishGoalVolume.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickInGameState.h"
#include "Character/BrickCharacter.h"
#include "GameFramework/Character.h"

// Components
#include "Components/BoxComponent.h"

AFinishGoalVolume::AFinishGoalVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AFinishGoalVolume::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AFinishGoalVolume::OnOverlapBegin);
	
}

void AFinishGoalVolume::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (ABrickCharacter* BrickCharacter = Cast<ABrickCharacter>(OtherActor))
	{
		if (ABrickGamePlayerState* PS = BrickCharacter->GetPlayerState<ABrickGamePlayerState>())
		{
			if (ABrickInGameState* GS = GetWorld()->GetGameState<ABrickInGameState>())
			{
				if (PS->GetTeam() == Team)
				{
					GS->RegisterGoalArrival(Team);
				}
			}
		}
	}

}

