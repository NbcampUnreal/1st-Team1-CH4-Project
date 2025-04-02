#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BrickGameMode.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABrickGameMode();

	void StartGame();
	void EndGame();
	void CheckWinCondition();

protected:
	int32 MaxPlayerCount;
	float GameDuration;
	bool bGameInProgress;
	TArray<APlayerController*> PlayerList;
};
