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
}

void AResultGameMode::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentLevel = GetWorld()->GetMapName();
    if (!CurrentLevel.Contains(TEXT("ResultLevel"))) return;

    FTimerHandle DelayHandle;
    GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]() {
        ABrickInGameState* GS = GetGameState<ABrickInGameState>();
        if (!GS) return;

        EGameTeam WinningTeam = GS->GetWinningTeam();

        TArray<AActor*> WinSpots;
        TArray<AActor*> LoseSpots;
        UGameplayStatics::GetAllActorsWithTag(this, FName("WinSpot"), WinSpots);
        UGameplayStatics::GetAllActorsWithTag(this, FName("LoseSpot"), LoseSpots);

        TArray<ABrickCharacter*> AllWinners;
        TArray<ABrickCharacter*> AllLosers;

        for (TActorIterator<ABrickCharacter> It(GetWorld()); It; ++It)
        {
            ABrickCharacter* Char = *It;
            if (!Char) continue;

            ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(Char->GetPlayerState());
            if (!PS) continue;

            if (PS->GetTeam() == WinningTeam)
                AllWinners.Add(Char);
            else
                AllLosers.Add(Char);
        }

        auto SortByPlayerID = [](const ABrickCharacter& A, const ABrickCharacter& B) {
            auto PSA = Cast<ABrickGamePlayerState>(A.GetPlayerState());
            auto PSB = Cast<ABrickGamePlayerState>(B.GetPlayerState());
            return PSA && PSB && PSA->GetBrickPlayerID() < PSB->GetBrickPlayerID();
            };

        AllWinners.Sort(SortByPlayerID);
        AllLosers.Sort(SortByPlayerID);

        TArray<ABrickCharacter*> WinningChars;
        TArray<ABrickCharacter*> LosingChars;

        // 승리팀 배치
        for (int32 i = 0; i < AllWinners.Num(); ++i)
        {
            if (!WinSpots.IsValidIndex(i)) break;

            ABrickCharacter* Char = AllWinners[i];
            if (!Char) continue;

            if (Char->HasAuthority())
            {
                Char->SetActorLocation(WinSpots[i]->GetActorLocation());
                Char->SetActorRotation(WinSpots[i]->GetActorRotation());
                Char->MulticastFixMeshRotation(FRotator(0.f, 0.f, 0.f));
            }

            Char->GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
            Char->AttachCrown();
            Char->PlayVictoryMontage();
            Char->SetMovementEnabled(false);

            WinningChars.Add(Char);

            FTimerHandle TempHandle;
            GetWorld()->GetTimerManager().SetTimer(TempHandle, FTimerDelegate::CreateWeakLambda(Char, [Char]() {
                Char->SetMovementEnabled(true);
                }), 3.0f, false);
        }

        // 패배팀 배치
        for (int32 i = 0; i < AllLosers.Num(); ++i)
        {
            if (!LoseSpots.IsValidIndex(i)) break;

            ABrickCharacter* Char = AllLosers[i];
            if (!Char || !Char->HasAuthority()) continue;

            Char->SetActorLocation(LoseSpots[i]->GetActorLocation());
            Char->SetActorRotation(LoseSpots[i]->GetActorRotation());
            Char->GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
            Char->PlayDefeatMontage();
            Char->SetMovementEnabled(false);

            LosingChars.Add(Char);
        }

        // 로컬 컨트롤러 캐릭터만 회전 보정
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (APlayerController* PC = Cast<APlayerController>(It->Get()))
            {
                ABrickCharacter* MyChar = Cast<ABrickCharacter>(PC->GetPawn());
                if (!MyChar) continue;

                ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(MyChar->GetPlayerState());
                if (!PS) continue;

                bool bIsWinner = (PS->GetTeam() == WinningTeam);
                int32 SpotIndex = bIsWinner ? AllWinners.Find(MyChar) : AllLosers.Find(MyChar);
                const TArray<AActor*>& SpotArray = bIsWinner ? WinSpots : LoseSpots;

                if (SpotArray.IsValidIndex(SpotIndex))
                {
                    MyChar->MulticastApplyFinalPose(SpotArray[SpotIndex]->GetActorRotation(), FRotator(0.f, 0.f, 0.f));
                }
            }
        }

        // 밀기 로직
        FTimerHandle PushCheckHandle;
        GetWorld()->GetTimerManager().SetTimer(PushCheckHandle, FTimerDelegate::CreateLambda([WinningChars, LosingChars]() {
            const float PushDistance = 100.f;
            const float PushStrength = 100.f;

            for (ABrickCharacter* Winner : WinningChars)
            {
                if (!Winner) continue;

                FVector WinnerLocation = Winner->GetActorLocation();
                FVector WinnerForward = Winner->GetActorForwardVector();

                for (ABrickCharacter* Loser : LosingChars)
                {
                    if (!Loser) continue;

                    FVector ToLoser = Loser->GetActorLocation() - WinnerLocation;
                    float Dist = ToLoser.Size();

                    if (Dist < PushDistance && FVector::DotProduct(WinnerForward, ToLoser.GetSafeNormal()) > 0.5f)
                    {
                        FVector PushVector = WinnerForward * PushStrength * 0.05f;
                        Loser->AddActorWorldOffset(PushVector, true);
                    }
                }
            }
            }), 0.05f, true);

        // 카메라 전환
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
                    PC->bAutoManageActiveCameraTarget = false;
                    PC->SetViewTargetWithBlend(ResultCamera, 1.0f);
                }
            }
        }
        }), 0.3f, false);
}
