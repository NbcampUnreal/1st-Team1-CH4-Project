#include "Network_Structure/BrickLobbyGameState.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/LobbyUserWidget.h"

ABrickLobbyGameState::ABrickLobbyGameState()
{
}

void ABrickLobbyGameState::NetMulticast_UpdateLobbyUI_Implementation()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(It->Get()))
        {
            if (PC->LobbyWidget)
            {
                FTimerHandle TimerHandle;
                PC->GetWorldTimerManager().SetTimer(TimerHandle, [=]()
                    {
                        PC->LobbyWidget->UpdateTeamInfo();
                    }, 1.0f, false);
            }
        }
    }
}


void ABrickLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}
