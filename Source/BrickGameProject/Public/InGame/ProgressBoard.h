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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitializeProgressLineYBounds();
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
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Arrow;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker")
	UTexture2D* RedDotTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker")
	UTexture2D* BlueDotTexture;


	TArray<UImage*> MarkerImages;
	TArray<ABrickGamePlayerState*> CachedPlayerStates;
	TMap<int32, UImage*> MarkerMap;

	FTimerHandle UpdateTimerHandle;

	bool bLineInitialized = false;


	float DotHalfSize = 20.f;
	float ProgressLineTopY;
	float ProgressLineBottomY;
};