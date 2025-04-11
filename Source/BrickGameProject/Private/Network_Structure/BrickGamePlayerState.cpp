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
	, ProgressRatio(0.f)
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

void ABrickGamePlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PlayerState))
	{
		BrickPS->BrickPlayerID = BrickPlayerID;
		BrickPS->bIsHost = bIsHost;
		BrickPS->Team = Team;
		BrickPS->bIsReady = bIsReady;
		BrickPS->CurrentCheckPoint = CurrentCheckPoint;
		BrickPS->bHasFinished = bHasFinished;
		BrickPS->FinishOrder = FinishOrder;
	}
}

void ABrickGamePlayerState::OnRep_ProgressRatio()
{
	//클라이언트 UI 업데이트 처리
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
