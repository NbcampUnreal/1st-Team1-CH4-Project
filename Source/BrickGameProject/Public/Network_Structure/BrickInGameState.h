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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RegisterGoalArrival(EGameTeam Team);

	EGameTeam GetWinningTeam() const { return WinningTeam; }
	void SetWinningTeam(EGameTeam NewWinner);

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const { return RemainingTime; }
	void SetRemainingTime(float Time); // 내부에서만 사용

	float GetTotalDistance() const { return FMath::Max(GoalY - StartY, 1.0f); }
protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_WinningTeam, BlueprintReadOnly)
	EGameTeam WinningTeam;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float RemainingTime;

	TMap<EGameTeam, int32> TeamGoalCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	float StartY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	float GoalY = 1000.f; // 예시값


	UFUNCTION()
	void OnRep_WinningTeam();
};
