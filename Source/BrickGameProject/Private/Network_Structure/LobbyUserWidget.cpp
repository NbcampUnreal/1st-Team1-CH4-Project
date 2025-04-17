#include "Network_Structure/LobbyUserWidget.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameState.h"
#include "Kismet/GameplayStatics.h"

//Components
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "InGame/LobbyUserEntry.h"


void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnClickReady);
	}
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnClickStart);
		StartButton->SetIsEnabled(false);
	}

	if (RedTeamButton)
	{
		RedTeamButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnClickRedTeam);
	}
	if (BlueTeamButton)
	{
		BlueTeamButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnClickBlueTeam);
	}

}

void ULobbyUserWidget::OnClickReady()
{
	ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(GetOwningPlayer());
	if (!PC)
		return;

	ABrickGamePlayerState* PS = Cast<ABrickGamePlayerState>(PC->PlayerState);
	if (!PS) 
		return;

	const bool bCurrentlyReady = PS->IsReady(); 
	const bool bNewReady = !bCurrentlyReady;

	PC->Server_SetReady(bNewReady);

	if (ReadyButtonText)
	{
		ReadyButtonText->SetText(FText::FromString(bNewReady ? TEXT("Cancel Ready") : TEXT("Ready")));
	}
	
}

void ULobbyUserWidget::OnClickStart()
{
	ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->Server_StartGame(); 
	}
}

void ULobbyUserWidget::OnClickRedTeam()
{
	if (ABrickGamePlayerController* BrickPC = Cast<ABrickGamePlayerController>(GetOwningPlayer()))
	{
		BrickPC->Server_SetTeam(EGameTeam::Red);
	}
}

void ULobbyUserWidget::OnClickBlueTeam()
{
	if (ABrickGamePlayerController* BrickPC = Cast<ABrickGamePlayerController>(GetOwningPlayer()))
	{
		BrickPC->Server_SetTeam(EGameTeam::Blue);
	}
}

void ULobbyUserWidget::ActivateStartButton(bool bEnable)
{
	if (StartButton)
	{
		StartButton->SetIsEnabled(bEnable);
	}
}

void ULobbyUserWidget::UpdateTeamInfo()
{
	VerticalBox_RedTeam->ClearChildren();
	VerticalBox_BlueTeam->ClearChildren();

	if (ABrickLobbyGameState* GS = GetWorld()->GetGameState<ABrickLobbyGameState>())
	{
		for (APlayerState* PS : GS->PlayerArray)
		{
			if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PS))
			{
				if (ULobbyUserEntry* UserEntry = CreateWidget<ULobbyUserEntry>(this, UserEntryWidgetClass))
				{
					UserEntry->SetPlayerInfo(BrickPS->GetBrickPlayerID(), BrickPS->IsReady());

					if (BrickPS->GetTeam() == EGameTeam::Red)
					{
						VerticalBox_RedTeam->AddChild(UserEntry);
					}
					else if (BrickPS->GetTeam() == EGameTeam::Blue)
					{
						VerticalBox_BlueTeam->AddChild(UserEntry);
					}
				}
			}
		}
	}
}
