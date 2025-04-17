#include "InGame/LobbyUserEntry.h"

//Components
#include "Components/TextBlock.h"

void ULobbyUserEntry::SetPlayerInfo(int32 PlayerID, bool bIsReady)
{
	TextBlock_PlayerID->SetText(FText::AsNumber(PlayerID));
	if (bIsReady)
	{
		TextBlock_ReadyStatus->SetText(FText::FromString(TEXT("Ready")));
		TextBlock_ReadyStatus->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TextBlock_ReadyStatus->SetVisibility(ESlateVisibility::Hidden);
	}
}
