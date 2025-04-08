#include "Network_Structure/BrickLobbyGameMode.h"
#include "Character/BrickCharacter.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickGameInstance.h"

ABrickLobbyGameMode::ABrickLobbyGameMode()
 : MaxPlayerCount(4)
	, MaxPlayersPerTeam(2)
	, TeamCounts()
	, NextPlayerID(0)
	, TeamOrder({ 
		EGameTeam::Red,
		EGameTeam::Blue,
		EGameTeam::Green,
		EGameTeam::Yellow 
		})

{
	bUseSeamlessTravel = true;
}

void ABrickLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(NewPlayer->PlayerState);
	if (!PS)
	{
		return;
	}
	
	PS->SetBrickPlayerID(NextPlayerID++);
	if (PS->GetBrickPlayerID() == 0)
	{
		PS->SetHost(true);
	}

	AssignTeam(NewPlayer);
	PlayerList.Add(NewPlayer);
	int32 PlayerID = PS->GetBrickPlayerID();
	EGameTeam Team = PS->GetTeam();

}

void ABrickLobbyGameMode::TryNotifyStartAvailable()
{
	if (!CheckAllPlayersReady())
		return;

	for (APlayerController* Player : PlayerList)
	{
		if (ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(Player))
		{
			if (ABrickGamePlayerState* PS = PC->GetBrickGamePlayerState())
			{
				if (PS->IsHost())
					PC->Client_EnableStartButton(true);
			}
		}
	}

}

void ABrickLobbyGameMode::StartGame()
{

	FString TargetMap = TEXT("/Game/Maps/InGameLevel?listen");
	GetWorld()->ServerTravel(TargetMap);
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




