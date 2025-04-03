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
	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
	int32 BrickPlayerID;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsHost;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
	EGameTeam Team;

	UPROPERTY(Replicated)
	bool bIsReady;
	
public:
	int32 GetPlayerId() const { return BrickPlayerID; }
	void SetPlayerId(int32 InPlayerId) { BrickPlayerID = InPlayerId; }

	bool IsHost() const { return bIsHost; }
	void SetHost(bool InIsHost) { bIsHost = InIsHost; }

	EGameTeam GetTeam() const { return Team; }
	void SetTeam(EGameTeam InTeam) { Team = InTeam; }

	bool IsReady() const { return bIsReady; }
	void SetReady(bool bReady) { bIsReady = bReady; }
};
