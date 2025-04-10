#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Net/UnrealNetwork.h"


ABrickInGameState::ABrickInGameState()
	: WinningTeam(EGameTeam::None)
{
	bReplicates = true;
}

void ABrickInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABrickInGameState, WinningTeam);
	DOREPLIFETIME(ABrickInGameState, RemainingTime);
}


void ABrickInGameState::RegisterGoalArrival(EGameTeam Team)
{
	int32& Count = TeamGoalCount.FindOrAdd(Team);
	Count++;

	if (Count >= 2)
	{
		// 승리 조건 만족 → GameMode에게 알리기
		Cast<ABrickInGameMode>(AuthorityGameMode)->HandleTeamWin(Team);
	}
}

void ABrickInGameState::SetWinningTeam(EGameTeam NewWinner)
{
	if (WinningTeam == NewWinner) return;

	WinningTeam = NewWinner;
}

void ABrickInGameState::SetRemainingTime(float Time)
{
	RemainingTime = Time;
}

void ABrickInGameState::OnRep_WinningTeam()
{
	UE_LOG(LogTemp, Warning, TEXT("Winning team replicated: %d"), (int32)WinningTeam);
}
