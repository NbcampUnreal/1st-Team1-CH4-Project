#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Network_Structure/EGameTeam.h"
#include "BrickInGameMode.generated.h"

class ABrickPlayerStart;

UCLASS()
class BRICKGAMEPROJECT_API ABrickInGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABrickInGameMode();

	virtual void BeginPlay() override;

	void AssignCheckPointForPlayers();

	FTimerHandle InitPlayerSpawnHandle;

protected:
	TMap<EGameTeam, ABrickPlayerStart*> StartPointsMap;

};
