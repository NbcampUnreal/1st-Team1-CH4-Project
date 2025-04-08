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
        UE_LOG(LogTemp, Warning, TEXT("Found ABrickPlayerStart: %s | TeamID: %d | Location: %s"),
            *Start->GetName(), (uint8)TeamID, *Start->GetActorLocation().ToString());

        // 이미 등록된 팀이면 건너뜀
        if (!StartPointsMap.Contains(TeamID))
        {
            UE_LOG(LogTemp, Warning, TEXT("-> Registered Team %d StartPoint: %s"), (uint8)TeamID, *Start->GetName());
            StartPointsMap.Add(TeamID, Start);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        InitPlayerSpawnHandle,
        this,
        &ABrickInGameMode::AssignCheckPointForPlayers,
        0.1f,
        false
    );
}

void ABrickInGameMode::PostLogin(APlayerController* NewPlayer)
{
}


void ABrickInGameMode::AssignCheckPointForPlayers()
{
    UE_LOG(LogTemp, Warning, TEXT(">> AssignCheckpointForPlayers called"));

    for (APlayerState* PS : GameState->PlayerArray)
    {
        if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PS))
        {
            int32 PlayerId = BrickPS->GetBrickPlayerID();
            EGameTeam Team = BrickPS->GetTeam();
            if (ABrickPlayerStart** FoundStart = StartPointsMap.Find(Team))
            {
                FVector StartLocation = (*FoundStart)->GetActorLocation();
                BrickPS->SetCurrentCheckPoint(StartLocation);
            }

            if (AController* PC = Cast<AController>(BrickPS->GetOwner()))
            {
                RestartPlayer(PC);
            }
        }

    }

   
}
