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
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void AssignCheckPointForPlayers();
	void HandleTeamWin(EGameTeam WinningTeam);

	//Timer
	FTimerHandle InitPlayerSpawnHandle;
	FTimerHandle TimerHandle_GameTimer;
	FTimerHandle ResultTravelHandle;
	FTimerHandle PhaseTimerHandle;

	//Timer Time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayTime")
	float PlacementPhaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayTime")
	float MaxGameTime; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayTime")
	float LoadingPhaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayTime")
	float IntroPhaseTime;

protected:
	void StartGameTimer();
	void TickGameTimer();

	void HandleWinByDistance();

	void TravelToResultLevel();

	void EnterPlacementPhase();
	void EnterCountdownPhase();
public:
	void EnterGameplayPhase();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameTeam WinningTeam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShowWiningUITime;

	TMap<EGameTeam, ABrickPlayerStart*> StartPointsMap;


};
