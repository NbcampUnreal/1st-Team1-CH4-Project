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
	void HandleTeamWin(EGameTeam WinningTeam);

	//Timer
	FTimerHandle InitPlayerSpawnHandle;
	FTimerHandle TimerHandle_GameTimer;
	FTimerHandle ResultTravelHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayTime")
	float MaxGameTime; 

protected:
	void StartGameTimer();     
	void TickGameTimer();       

	void HandleWinByDistance();

	void TravelToResultLevel();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameTeam WinningTeam;

	TMap<EGameTeam, ABrickPlayerStart*> StartPointsMap;

};
