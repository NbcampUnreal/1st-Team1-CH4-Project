#include "Network_Structure/BrickLobbyGameMode.h"
#include "Character/BricCharacter.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"

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
	DefaultPawnClass = ABricCharacter::StaticClass(); 
	PlayerControllerClass = ABrickGamePlayerController::StaticClass();

	bUseSeamlessTravel = true;
}

void ABrickLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("PostLogin È£ÃâµÊ: %s"), *NewPlayer->GetName());

	ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(NewPlayer->PlayerState);
	if (!PS)
		return;

	PS->SetPlayerId(NextPlayerID++);
	if (PS->GetPlayerId() == 0)
	{
		PS->SetHost(true);
	}

	AssignTeam(NewPlayer);
	PlayerList.Add(NewPlayer);
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




