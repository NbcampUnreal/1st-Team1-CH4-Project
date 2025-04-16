#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

class UProgressBoard;

UCLASS()
class BRICKGAMEPROJECT_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBoard* ProgressBoard;
};
