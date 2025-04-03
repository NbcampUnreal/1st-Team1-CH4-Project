#include "Network_Structure/BrickGamePlayerState.h"
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
