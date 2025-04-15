#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Character/BrickCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ABrickInGameState::ABrickInGameState()
	: WinningTeam(EGameTeam::None)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ABrickInGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> StartActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("RedStart"), StartActors);

	if (StartActors.Num() > 0)
	{
		StartY = StartActors[0]->GetActorLocation().Y;
	}

	// Goal Actor 찾기
	TArray<AActor*> GoalActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("RedGoal"), GoalActors);

	if (GoalActors.Num() > 0)
	{
		GoalY = GoalActors[0]->GetActorLocation().Y;
	}
}

void ABrickInGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (APlayerState* PS : PlayerArray)
	{
		if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PS))
		{
			ABrickCharacter* Character = Cast<ABrickCharacter>(BrickPS->GetPawn());
			if (Character)
			{
				float ProgressRatio = (Character->GetActorLocation().Y - StartY) / (GoalY - StartY);
				ProgressRatio = FMath::Clamp(ProgressRatio, 0.0f, 1.0f);
				BrickPS->SetProgressRatio(ProgressRatio);
				UE_LOG(LogTemp, Warning, TEXT("Progress Ratio: %f"), ProgressRatio);
			}
		}
	}
}

void ABrickInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABrickInGameState, WinningTeam);
	DOREPLIFETIME(ABrickInGameState, RemainingTime);
	DOREPLIFETIME(ABrickInGameState, CurrentPhase);
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

void ABrickInGameState::OnRep_GamePhase()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ABrickGamePlayerController* BPC = Cast<ABrickGamePlayerController>(PC))
		{
			if (CurrentPhase == EGamePhase::Intro)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Intro"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->PlayIntroCameraSequence(); // 로컬 플레이어만 카메라 전환
				}
			}
			else if (CurrentPhase == EGamePhase::Placement)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Placement"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->ResetCameras();
					BPC->PlayPlacementCameraSequence(true); // 로컬 플레이어만 카메라 전환
					BPC->InitTrapSettingUI();
				}
			}
			else if (CurrentPhase == EGamePhase::Gameplay)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Gameplay"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->ReturnToPawnCamera();
					BPC->InitInGameUI();
				}
			}
		}
	}
}

void ABrickInGameState::SetGamePhase(EGamePhase NewPhase)
{
	CurrentPhase = NewPhase;

	// 플레이어 컨트롤러가 로컬 플레이어 컨트롤러인지 확인
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 인덱스 0번은 로컬 플레이어의 컨트롤러
	if (PlayerController && PlayerController->IsLocalPlayerController() && HasAuthority())
	{
		OnRep_GamePhase();  // 로컬 플레이어에게만 실행
	}
}
