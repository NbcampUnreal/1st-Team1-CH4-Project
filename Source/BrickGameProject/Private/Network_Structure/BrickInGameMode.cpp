#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickGameInstance.h"
#include "Network_Structure/EGameTeam.h"
#include "Character/BrickCharacter.h"
#include "InGame/BrickPlayerStart.h"
#include "EngineUtils.h"

ABrickInGameMode::ABrickInGameMode()
	:bGameOver(false)
	, WinningTeam(EGameTeam::None)
{
    bUseSeamlessTravel = true;
	MaxGameTime = 30.f; 

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
        2.0f,
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

    // Update GamePhase(Intro)
    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        GS->SetGamePhase(EGamePhase::Intro);
    }

    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ABrickInGameMode::EnterPlacementPhase, 6.0f, false);
}

void ABrickInGameMode::HandleTeamWin(EGameTeam WinnerTeam)
{
    if (!bGameOver) return;

	bGameOver = true;

    if (ABrickInGameState* GS = GetWorld()->GetGameState<ABrickInGameState>())
    {
        GS->SetWinningTeam(WinnerTeam);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            UE_LOG(LogTemp, Warning, TEXT("Team %d wins!"), (int32)WinnerTeam);
			//Client RPC to show the winning UI
        }
    }

    GetWorldTimerManager().SetTimer(
        ResultTravelHandle,
        this,
        &ABrickInGameMode::TravelToResultLevel,
        5.0f,
        false
    );
}



void ABrickInGameMode::StartGameTimer()
{
    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        GS->SetRemainingTime(MaxGameTime);
    }

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_GameTimer, this, &ABrickInGameMode::TickGameTimer, 0.1f, true);
}

void ABrickInGameMode::TickGameTimer()
{
    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        float RemainingTime = GS->GetRemainingTime();
        RemainingTime -= 0.1f;

		GS->SetRemainingTime(RemainingTime);

		if (RemainingTime <= 0.0f)
		{
            UE_LOG(LogTemp, Warning, TEXT("Time Over"));

			bGameOver = true;
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GameTimer);

            HandleWinByDistance(); 
		}
    }
}

void ABrickInGameMode::HandleWinByDistance()
{
    ABrickCharacter* FurthestCharacter = nullptr;
    float MaxDistance = -FLT_MAX;

    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PS))
            {
                APawn* Pawn = BrickPS->GetPawn();
                if (ABrickCharacter* Character = Cast<ABrickCharacter>(Pawn))
                {
                    FVector Location = Character->GetActorLocation();
                    if (Location.Y > MaxDistance) // Y�� ����
                    {
                        MaxDistance = Location.Y;
                        FurthestCharacter = Character;
                    }
                }
            }
        }
    }

    if (FurthestCharacter)
    {
        ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(FurthestCharacter->GetPlayerState());
        if (PS)
        {
            WinningTeam = PS->GetTeam();
			HandleTeamWin(WinningTeam);
        }
    }
}

void ABrickInGameMode::TravelToResultLevel()
{
    GetWorld()->ServerTravel("/Game/Maps/ResultLevel?listen");
}

void ABrickInGameMode::EnterPlacementPhase()
{
    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        GS->SetGamePhase(EGamePhase::Placement);
    }

    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ABrickInGameMode::EnterGameplayPhase, 20.0f, false);
}

void ABrickInGameMode::EnterGameplayPhase()
{
    if (ABrickInGameState* GS = GetGameState<ABrickInGameState>())
    {
        GS->SetGamePhase(EGamePhase::Gameplay);
    }

    StartGameTimer();
}


