#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ResultGameMode.generated.h"

UCLASS()
class BRICKGAMEPROJECT_API AResultGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AResultGameMode();

	virtual void BeginPlay() override;

};
