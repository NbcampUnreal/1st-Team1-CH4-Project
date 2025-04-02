#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

UCLASS()
class BRICKGAMEPROJECT_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABaseGameMode();

	void StartGame();                     
	void EndGame();                      
	void CheckWinCondition();             
	void RespawnPlayer(AController* Player); 


protected:
	int32 MaxPlayerCount;         
	float GameDuration;          
	bool bGameInProgress;
	TArray<APlayerController*> PlayerList;

};
