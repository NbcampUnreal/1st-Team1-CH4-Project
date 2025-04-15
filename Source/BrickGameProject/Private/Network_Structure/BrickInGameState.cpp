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

	// Goal Actor ã��
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
	DOREPLIFETIME(ABrickInGameState, Countdown);
}


void ABrickInGameState::RegisterGoalArrival(EGameTeam Team)
{
	int32& Count = TeamGoalCount.FindOrAdd(Team);
	Count++;

	if (Count >= 2)
	{
		// �¸� ���� ���� �� GameMode���� �˸���
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

void ABrickInGameState::OnRep_Countdown()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ABrickGamePlayerController* BPC = Cast<ABrickGamePlayerController>(PC))
		{
			BPC->UpdateCountdownUI(Countdown);
		}
	}
}

void ABrickInGameState::StartCountdown()
{
	if (!HasAuthority()) return;

	Countdown = 3;
	OnRep_Countdown(); // For Server
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABrickInGameState::CountdownTick, 1.0f, true);
}

void ABrickInGameState::CountdownTick()
{
	Countdown--;
	if (Countdown <= 0)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		if (HasAuthority()) // ���������� ����
		{
			if (ABrickInGameMode* GM = Cast<ABrickInGameMode>(GetWorld()->GetAuthGameMode()))
			{
				GM->EnterGameplayPhase();
			}
		}
	}

	OnRep_Countdown(); // For Client
}

void ABrickInGameState::OnRep_GamePhase()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ABrickGamePlayerController* BPC = Cast<ABrickGamePlayerController>(PC))
		{
			if (CurrentPhase == EGamePhase::Loading)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Loading"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->SetIgnoreMoveInput(true);
					if (APawn* Pawn = BPC->GetPawn())
					{
						if (ABrickCharacter* MyCharacter = Cast<ABrickCharacter>(Pawn))
						{
							MyCharacter->SetCanUseSkill(false);
						}
					}
				}
			}
			else if (CurrentPhase == EGamePhase::Intro)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Intro"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->PlayIntroCameraSequence(); // ���� �÷��̾ ī�޶� ��ȯ
				}
			}
			else if (CurrentPhase == EGamePhase::Placement)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Placement"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->ResetCameras();
					BPC->PlayPlacementCameraSequence(true); // ���� �÷��̾ ī�޶� ��ȯ
					BPC->InitTrapSettingUI();
				}
			}
			else if (CurrentPhase == EGamePhase::Countdown)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Countdown"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->ReturnToPawnCamera();
					BPC->SetIgnoreMoveInput(true);
					BPC->InitCountdownUI();
				}
			}
			else if (CurrentPhase == EGamePhase::Gameplay)
			{
				UE_LOG(LogTemp, Warning, TEXT("EGamePhase : Gameplay"));
				if (BPC->IsLocalPlayerController())
				{
					BPC->InitInGameUI();
					BPC->SetIgnoreMoveInput(false);
					if (APawn* Pawn = BPC->GetPawn())
					{
						if (ABrickCharacter* MyCharacter = Cast<ABrickCharacter>(Pawn))
						{
							MyCharacter->SetCanUseSkill(true);
						}
					}
				}
			}
		}
	}
}

void ABrickInGameState::SetGamePhase(EGamePhase NewPhase)
{
	CurrentPhase = NewPhase;

	// �÷��̾� ��Ʈ�ѷ��� ���� �÷��̾� ��Ʈ�ѷ����� Ȯ��
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �ε��� 0���� ���� �÷��̾��� ��Ʈ�ѷ�
	if (PlayerController && PlayerController->IsLocalPlayerController() && HasAuthority())
	{
		OnRep_GamePhase();  // ���� �÷��̾�Ը� ����
	}
}

EGamePhase ABrickInGameState::GetCurrentPhase()
{
	return CurrentPhase;
}
