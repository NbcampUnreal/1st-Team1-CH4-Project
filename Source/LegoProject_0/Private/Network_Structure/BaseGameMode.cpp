#include "Network_Structure/BaseGameMode.h"
#include "Network_Structure/BaseGameState.h"
#include "Character/LegoPlayerController.h"
#include "Net/UnrealNetwork.h"

ABaseGameMode::ABaseGameMode()
	: MaxPlayerCount(4)
	, GameDuration(300.0f)
	, bGameInProgress(false)
{
}
