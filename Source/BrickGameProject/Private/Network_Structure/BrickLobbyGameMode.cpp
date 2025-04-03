#include "Network_Structure/BrickLobbyGameMode.h"
#include "Character/BricCharacter.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"

ABrickLobbyGameMode::ABrickLobbyGameMode()
	: MaxPlayerCount(4)
	, MaxPlayersPerTeam(2)
	, TeamCounts()
	, NextPlayerID(0)
	, GameDuration(300.0f)
	, bGameInProgress(false)
	, TeamOrder({ 
		EGameTeam::Red,
		EGameTeam::Blue,
		EGameTeam::Green,
		EGameTeam::Yellow 
		})

{
	DefaultPawnClass = ABricCharacter::StaticClass(); 
	PlayerControllerClass = ABrickGamePlayerController::StaticClass();

}

void ABrickLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(NewPlayer->PlayerState);
	if (!PS)
		return;

	PS->SetPlayerId(PlayerList.Num());
	if (PlayerList.Num() == 0)
	{
		PS->SetHost(true);
	}
	else
	{
		PS->SetHost(false);
	}
	AssignTeam(NewPlayer);
	PlayerList.Add(NewPlayer);
}


void ABrickLobbyGameMode::AssignTeam(APlayerController* NewPlayer)
{
	ABrickGamePlayerState* PlayerState = NewPlayer->GetPlayerState<ABrickGamePlayerState>();
	if (!PlayerState) return;

	for (EGameTeam Team: TeamOrder)
	{
		int32 CurrentCount = TeamCounts.FindRef(Team);
		if (CurrentCount < MaxPlayersPerTeam)
		{
			PlayerState->SetTeam(Team);
			TeamCounts.FindOrAdd(Team)++; 
			return;
		}
	}
}

bool ABrickLobbyGameMode::CheckAllPlayersReady()
{
	for (APlayerController* Player : PlayerList)
	{
		if (ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(Player))
		{
			if (ABrickGamePlayerState* PS = PC->GetBrickGamePlayerState())
			{
				if (!PS->IsReady())
				{
					return false;
				}
			}
		}
	}

	return true;
}



void ABrickLobbyGameMode::StartGame()
{
}

void ABrickLobbyGameMode::EndGame()
{
}

void ABrickLobbyGameMode::CheckWinCondition()
{
}

