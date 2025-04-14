#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Network_Structure/EGameTeam.h"
#include "BrickGamePlayerState.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABrickGamePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

	UFUNCTION()
	void OnRep_ProgressRatio();
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
	int32 BrickPlayerID;

	//Lobby
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsHost;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
	EGameTeam Team;

	UPROPERTY(Replicated)
	bool bIsReady;
	
	//InGame
	UPROPERTY(Replicated)
	FVector CurrentCheckPoint;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ProgressRatio)
	float ProgressRatio;

	UPROPERTY(Replicated)
	bool bHasFinished;

	UPROPERTY(Replicated)
	int32 FinishOrder;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetBrickPlayerID() const { return BrickPlayerID; }
	UFUNCTION(BlueprintCallable)
	void SetBrickPlayerID(int32 InPlayerId) { BrickPlayerID = InPlayerId; }

	UFUNCTION(BlueprintCallable)
	bool IsHost() const { return bIsHost; }
	UFUNCTION(BlueprintCallable)
	void SetHost(bool InIsHost) { bIsHost = InIsHost; }

	UFUNCTION(BlueprintCallable)
	EGameTeam GetTeam() const { return Team; }
	UFUNCTION(BlueprintCallable)
	void SetTeam(EGameTeam InTeam) { Team = InTeam; }
	UFUNCTION(BlueprintCallable)
	bool IsReady() const { return bIsReady; }
	UFUNCTION(BlueprintCallable)
	void SetReady(bool bReady);

	UFUNCTION(BlueprintCallable)
	FVector GetCurrentCheckPoint() const { return CurrentCheckPoint; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentCheckPoint(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	float GetProgressRatio() const { return ProgressRatio; }
	UFUNCTION(BlueprintCallable)
	void SetProgressRatio(float InProgressRatio) { ProgressRatio = InProgressRatio; }

};
