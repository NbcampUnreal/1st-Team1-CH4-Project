#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EGameTeam.h"
#include "BrickLobbyGameMode.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABrickLobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	void AssignTeam(APlayerController* NewPlayer);
	bool CheckAllPlayersReady();
	void StartGame();
	void EndGame();
	void CheckWinCondition();

	//Game Setup
	UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
	int32 MaxPlayerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
	int32 MaxPlayersPerTeam;

	UPROPERTY(VisibleAnywhere, Category = "Game Setup")
	TMap<EGameTeam, int32> TeamCounts;

	int32 NextPlayerID;

	UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
	float GameDuration;

	bool bGameInProgress;

	TArray<APlayerController*> PlayerList;


	TArray<EGameTeam> TeamOrder;
};
