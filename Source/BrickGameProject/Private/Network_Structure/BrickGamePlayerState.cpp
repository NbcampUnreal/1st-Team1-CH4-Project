#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Net/UnrealNetwork.h"

ABrickGamePlayerState::ABrickGamePlayerState()
	: BrickPlayerID(0)
	, bIsHost(false)
	, Team(EGameTeam::None)
	, bIsReady(false)
	, CurrentCheckPoint(FVector::ZeroVector)
	, bHasFinished(false)
	, FinishOrder(-1)
{
	bReplicates = true;
	bAlwaysRelevant = true;

}

void ABrickGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABrickGamePlayerState, BrickPlayerID);
	DOREPLIFETIME(ABrickGamePlayerState, bIsHost);
	DOREPLIFETIME(ABrickGamePlayerState, Team);
	DOREPLIFETIME(ABrickGamePlayerState, bIsReady);
	DOREPLIFETIME(ABrickGamePlayerState, CurrentCheckPoint);
	DOREPLIFETIME(ABrickGamePlayerState, bHasFinished);
	DOREPLIFETIME(ABrickGamePlayerState, FinishOrder);
}

void ABrickGamePlayerState::SetReady(bool bReady)
{
	bIsReady = bReady;

	if (HasAuthority())
	{
		ABrickLobbyGameMode* GM = GetWorld()->GetAuthGameMode<ABrickLobbyGameMode>();
		if (GM)
		{
			GM->TryNotifyStartAvailable();
		}
	}
}

void ABrickGamePlayerState::SetCurrentCheckPoint(const FVector& Location)
{
	CurrentCheckPoint = Location;
}
