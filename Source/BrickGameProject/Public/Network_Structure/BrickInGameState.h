#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Network_Structure/EGameTeam.h"
#include "BrickInGameState.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickInGameState : public AGameState
{
	GENERATED_BODY()
public:
	ABrickInGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RegisterGoalArrival(EGameTeam Team);

	EGameTeam GetWinningTeam() const { return WinningTeam; }
	void SetWinningTeam(EGameTeam NewWinner);

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const { return RemainingTime; }
	void SetRemainingTime(float Time); // 내부에서만 사용

protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_WinningTeam, BlueprintReadOnly)
	EGameTeam WinningTeam;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float RemainingTime;

	TMap<EGameTeam, int32> TeamGoalCount;

	UFUNCTION()
	void OnRep_WinningTeam();
};
