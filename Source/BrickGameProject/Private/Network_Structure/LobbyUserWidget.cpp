#include "Network_Structure/LobbyUserWidget.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"

//Components
#include "Components/Button.h"
#include "Components/TextBlock.h"

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

void ULobbyUserWidget::ActivateStartButton()
{
	if (StartButton)
	{
		StartButton->SetIsEnabled(true);
	}
}
