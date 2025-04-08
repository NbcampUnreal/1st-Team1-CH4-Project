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

	UFUNCTION()
	void TryNotifyStartAvailable();
	UFUNCTION()
	void StartGame();

protected:
	void AssignTeam(APlayerController* NewPlayer);
	bool CheckAllPlayersReady();


	UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
	int32 MaxPlayerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Setup")
	int32 MaxCountPerTeam;

	UPROPERTY(VisibleAnywhere, Category = "Game Setup")
	TMap<EGameTeam, int32> PlayersPerTeam;

	int32 NextPlayerID;


	TArray<APlayerController*> PlayerList;


	TArray<EGameTeam> TeamOrder;
};
