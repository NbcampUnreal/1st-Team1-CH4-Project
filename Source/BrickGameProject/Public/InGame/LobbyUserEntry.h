#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserEntry.generated.h"

class UTextBlock;

UCLASS()
class BRICKGAMEPROJECT_API ULobbyUserEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerInfo(int32 PlayerID, bool bIsReady);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_PlayerID;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ReadyStatus;
};
