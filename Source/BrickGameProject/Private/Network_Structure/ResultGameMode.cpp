#include "Network_Structure/ResultGameMode.h"
#include "Network_Structure/BrickInGameState.h"
#include "Character/BrickCharacter.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

AResultGameMode::AResultGameMode()
{
    DefaultPawnClass = nullptr;
}

void AResultGameMode::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentLevel = GetWorld()->GetMapName();
    if (!CurrentLevel.Contains(TEXT("ResultLevel"))) return;

    // ✅ 0.3초 후 처리
    FTimerHandle DelayHandle;
    GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]()
        {
            ABrickInGameState* GS = GetGameState<ABrickInGameState>();
            if (!GS) return;

            EGameTeam WinningTeam = GS->GetWinningTeam();

            // 위치
            TArray<AActor*> WinSpots;
            TArray<AActor*> LoseSpots;
            UGameplayStatics::GetAllActorsWithTag(this, FName("WinSpot"), WinSpots);
            UGameplayStatics::GetAllActorsWithTag(this, FName("LoseSpot"), LoseSpots);

            int32 WinIndex = 0;
            int32 LoseIndex = 0;

            for (TActorIterator<ABrickCharacter> It(GetWorld()); It; ++It)
            {
                ABrickCharacter* Char = *It;
                if (!Char) continue;

                ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(Char->GetPlayerState());
                if (!PS) continue;

                bool bIsWinner = (PS->GetTeam() == WinningTeam);

                if (bIsWinner && WinSpots.IsValidIndex(WinIndex))
                {
                    Char->SetActorLocation(WinSpots[WinIndex]->GetActorLocation());
                    Char->SetActorRotation(WinSpots[WinIndex]->GetActorRotation());
                    Char->AttachCrown();
                    Char->PlayVictoryMontage();
                    WinIndex++;
                }
                else if (!bIsWinner && LoseSpots.IsValidIndex(LoseIndex))
                {
                    Char->SetActorLocation(LoseSpots[LoseIndex]->GetActorLocation());
                    Char->SetActorRotation(LoseSpots[LoseIndex]->GetActorRotation());
                    Char->PlayDefeatMontage();
                    LoseIndex++;
                }
            }

            // 카메라
            ACameraActor* ResultCamera = nullptr;
            for (TActorIterator<ACameraActor> CamIt(GetWorld()); CamIt; ++CamIt)
            {
                if (CamIt->ActorHasTag(FName("ResultCamera")))
                {
                    ResultCamera = *CamIt;
                    break;
                }
            }

            if (ResultCamera)
            {
                for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
                {
                    if (APlayerController* PC = It->Get())
                    {
                        PC->bAutoManageActiveCameraTarget = false; // ✅ 자동 전환 막기
                        PC->SetViewTargetWithBlend(ResultCamera, 1.0f);
                    }
                }
            }

        }), 0.3f, false);
}