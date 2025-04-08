#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Network_Structure/EGameTeam.h"
#include "BrickPlayerStart.generated.h"




UCLASS()
class BRICKGAMEPROJECT_API ABrickPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	EGameTeam TeamID;
};
