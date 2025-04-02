#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

UCLASS()
class LEGOPROJECT_0_API ABaseGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABaseGameState();


protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
