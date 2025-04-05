#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Net/UnrealNetwork.h"

ABrickGamePlayerState::ABrickGamePlayerState()
	: BrickPlayerID(0)
	, bIsHost(false)
	, Team(EGameTeam::None)
	, bIsReady(false)
{


}

void ABrickGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABrickGamePlayerState, BrickPlayerID);
	DOREPLIFETIME(ABrickGamePlayerState, bIsHost);
	DOREPLIFETIME(ABrickGamePlayerState, Team);
	DOREPLIFETIME(ABrickGamePlayerState, bIsReady);
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
