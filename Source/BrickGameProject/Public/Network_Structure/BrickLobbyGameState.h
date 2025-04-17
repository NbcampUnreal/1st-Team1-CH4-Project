#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BrickLobbyGameState.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickLobbyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABrickLobbyGameState();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdateLobbyUI();

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
