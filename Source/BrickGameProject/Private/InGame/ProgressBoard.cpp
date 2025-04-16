#include "InGame/ProgressBoard.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerState.h"
//Components
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UProgressBoard::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UProgressBoard::InitializeProgressLineYBounds);

	MarkerImages = { Image_Marker1, Image_Marker2, Image_Marker3, Image_Marker4 };

	if (ABrickInGameState* BrickGS = GetWorld()->GetGameState<ABrickInGameState>())
	{
		for (APlayerState* PS : BrickGS->PlayerArray)
		{
			if (ABrickGamePlayerState* BrickPS = Cast<ABrickGamePlayerState>(PS))
			{
				CachedPlayerStates.Add(BrickPS);
			}
		}
	}

	for (int32 i = 0; i < CachedPlayerStates.Num(); ++i)
	{
		ABrickGamePlayerState* PS = CachedPlayerStates[i];
		if (MarkerImages.IsValidIndex(i))
		{
			MarkerMap.Add(PS->GetBrickPlayerID(), MarkerImages[i]);

			MarkerImages[i]->SetColorAndOpacity(GetColorByTeam(PS->GetTeam()));
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerHandle,
		this,
		&UProgressBoard::UpdateMarkerPositions,
		0.2f,
		true
	);
}


FLinearColor UProgressBoard::GetColorByTeam(EGameTeam Team) const
{
	switch (Team)
	{
	case EGameTeam::Red:
		return FLinearColor::Red;
	case EGameTeam::Blue:
		return FLinearColor::Blue;
	default:
		return FLinearColor::White;
	}
}

void UProgressBoard::InitializeProgressLineYBounds()
{
	if (UCanvasPanelSlot* LineSlot = Cast<UCanvasPanelSlot>(Image_ProgressLine->Slot))
	{
		FVector2D RenderSize = Image_ProgressLine->GetCachedGeometry().GetLocalSize();

		float DotHalfSize = 10.f;
		ProgressLineTopY = 0.f + DotHalfSize;
		ProgressLineBottomY = RenderSize.Y - DotHalfSize;

	}
}

void UProgressBoard::UpdateMarkerPositions()
{
	for (int32 i = 0; i < CachedPlayerStates.Num(); ++i)
	{
		float ProgressRatio = CachedPlayerStates[i]->GetProgressRatio();
		float MarkerY = FMath::Lerp(ProgressLineBottomY, ProgressLineTopY, ProgressRatio);

		if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(MarkerImages[i]->Slot))
		{
			FVector2D Pos = MarkerSlot->GetPosition();
			Pos.Y = MarkerY;
			MarkerSlot->SetPosition(Pos);
		}
	}
}

