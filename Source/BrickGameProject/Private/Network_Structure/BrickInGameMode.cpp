#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickGameInstance.h"
#include "Network_Structure/EGameTeam.h"
#include "InGame/BrickPlayerStart.h"
#include "EngineUtils.h"

ABrickInGameMode::ABrickInGameMode()
{
    bUseSeamlessTravel = true;
}


void ABrickInGameMode::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<ABrickPlayerStart> It(GetWorld()); It; ++It)
    {
        ABrickPlayerStart* Start = *It;
        if (!Start) continue;

        EGameTeam TeamID = Start->TeamID;
        if (!StartPointsMap.Contains(TeamID))
        {
            StartPointsMap.Add(TeamID, Start);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        InitPlayerSpawnHandle,
        this,
        &ABrickInGameMode::AssignCheckPointForPlayers,
        1.0f,
        false
    );
}


void ABrickInGameMode::AssignCheckPointForPlayers()
{

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            if (ABrickGamePlayerState* PS = PC->GetPlayerState<ABrickGamePlayerState>())
            {
                EGameTeam Team = PS->GetTeam();

                if (AActor* StartPoint = StartPointsMap.FindRef(Team))
                {
                    if (APawn* Pawn = PC->GetPawn())
                    {
                        Pawn->SetActorLocation(StartPoint->GetActorLocation());
						PS->SetCurrentCheckPoint(StartPoint->GetActorLocation());
                    }
                }
            }
        }
    }
}
