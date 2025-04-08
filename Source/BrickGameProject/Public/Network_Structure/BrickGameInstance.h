#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EGameTeam.h"
#include "BrickGameInstance.generated.h"


UCLASS()
class BRICKGAMEPROJECT_API UBrickGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 MyPlayerId;

	UPROPERTY()
	EGameTeam MyTeam;

	
};
