#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BrickGameState.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API ABrickGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABrickGameState();

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
