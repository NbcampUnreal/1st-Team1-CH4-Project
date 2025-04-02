#include "Network_Structure/BaseGameState.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
