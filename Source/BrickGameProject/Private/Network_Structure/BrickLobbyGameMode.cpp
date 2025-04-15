#include "Network_Structure/BrickLobbyGameMode.h"
#include "Character/BrickCharacter.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickLobbyGameState.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickGameInstance.h"

ABrickLobbyGameMode::ABrickLobbyGameMode()
	: MaxPlayerCount(4)
	, MaxCountPerTeam(2)
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

	if (ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(NewPlayer->PlayerState))
	{
		PS->SetBrickPlayerID(NextPlayerID++);
		if (PS->GetBrickPlayerID() == 0)
		{
			PS->SetHost(true);
		}

		AssignTeam(NewPlayer);
		PlayerList.Add(NewPlayer);

		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]()
			{
				if (ABrickLobbyGameState* GS = GetGameState<ABrickLobbyGameState>())
				{
					GS->NetMulticast_UpdateLobbyUI();
				}
			}, 1.0f, false); 

	}

}

void ABrickLobbyGameMode::TryNotifyStartAvailable()
{
	if (!CheckAllPlayersReady() || !CheckTeamsFull())
	{
		for (APlayerController* Player : PlayerList)
		{
			if (ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(Player))
			{
				if (ABrickGamePlayerState* PS = PC->GetBrickGamePlayerState())
				{
					if (PS->IsHost())
						PC->Client_EnableStartButton(false);
				}
			}
		}

		return;
	}



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
		int32 CurrentCount = PlayersPerTeam.FindRef(Team);
		if (CurrentCount < MaxCountPerTeam)
		{
			PlayerState->SetTeam(Team);
			int32& Count = PlayersPerTeam.FindOrAdd(Team);
			Count++;
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

bool ABrickLobbyGameMode::CheckTeamsFull()
{
	int32 RedCount = 0;
	int32 BlueCount = 0;

	for (APlayerController* PC : PlayerList)
	{
		if (ABrickGamePlayerController* BrickPC = Cast<ABrickGamePlayerController>(PC))
		{
			if (ABrickGamePlayerState* PS = BrickPC->GetBrickGamePlayerState())
			{
				if (PS->GetTeam() == EGameTeam::Red)
				{
					RedCount++;
				}
				else if (PS->GetTeam() == EGameTeam::Blue)
				{
					BlueCount++;
				}
			}
		}
	}

	return (RedCount == 2 && BlueCount == 2);  
}
