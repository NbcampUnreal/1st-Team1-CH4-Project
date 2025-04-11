#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EGameTeam.h"
#include "Interfaces/OnlineSessionInterface.h"
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

	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession(const FString& IPAddress);

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void CreateSessionInternal();
	virtual void Init() override;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	IOnlineSessionPtr SessionInterface;
	
};
