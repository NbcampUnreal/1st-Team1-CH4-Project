#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Network_Structure/EGameTeam.h"
#include "ProgressBoard.generated.h"

class ABrickGamePlayerState;
class UImage;

UCLASS()
class BRICKGAMEPROJECT_API UProgressBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FLinearColor GetColorByTeam(EGameTeam Team) const;
	void UpdateMarkerPositions();

	UPROPERTY(meta = (BindWidget))
	UImage* Image_ProgressLine;
	UPROPERTY(meta = (BindWidget)) 
	UImage* Image_Marker1;
	UPROPERTY(meta = (BindWidget)) 
	UImage* Image_Marker2;
	UPROPERTY(meta = (BindWidget)) 
	UImage* Image_Marker3;
	UPROPERTY(meta = (BindWidget)) 
	UImage* Image_Marker4;

	TArray<UImage*> MarkerImages;
	TArray<ABrickGamePlayerState*> CachedPlayerStates;
	TMap<int32, UImage*> MarkerMap;

	FTimerHandle UpdateTimerHandle;


	float ProgressLineTopY;
	float ProgressLineBottomY;
};