#include "InGame/ProgressBoard.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGamePlayerState.h"
//Components
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UProgressBoard::NativeConstruct()
{
	Super::NativeConstruct();



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

void UProgressBoard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bLineInitialized)
	{
		FVector2D Size = Image_ProgressLine->GetCachedGeometry().GetLocalSize();

		if (Size.Y > 10.f) // 또는 0.f 초과
		{
			ProgressLineTopY = DotHalfSize;
			ProgressLineBottomY = Size.Y - DotHalfSize;

			bLineInitialized = true;

			UpdateMarkerPositions(); // 정확한 좌표로 표시
		}
	}
}



void UProgressBoard::InitializeProgressLineYBounds()
{
	if (UCanvasPanelSlot* LineSlot = Cast<UCanvasPanelSlot>(Image_ProgressLine->Slot))
	{
		FVector2D RenderSize = Image_ProgressLine->GetCachedGeometry().GetLocalSize();

		ProgressLineTopY = 0.f + DotHalfSize;
		ProgressLineBottomY = RenderSize.Y - DotHalfSize;

	}
}

void UProgressBoard::UpdateMarkerPositions()
{
	ABrickGamePlayerState* MyPS = GetOwningPlayerState<ABrickGamePlayerState>();
	if (!MyPS) return;

	const int32 MyPlayerId = MyPS->GetBrickPlayerID();

	for (int32 i = 0; i < CachedPlayerStates.Num(); ++i)
	{
		ABrickGamePlayerState* BrickPS = CachedPlayerStates[i];
		if (!BrickPS || !MarkerImages.IsValidIndex(i) || !MarkerImages[i]) continue;

		const float ProgressRatio = BrickPS->GetProgressRatio();
		const float MarkerY = FMath::Lerp(ProgressLineBottomY, ProgressLineTopY, ProgressRatio);

		if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(MarkerImages[i]->Slot))
		{
			FVector2D Pos = MarkerSlot->GetPosition();
			Pos.Y = MarkerY;
			MarkerSlot->SetPosition(Pos);

			// Arrow 위치 갱신 (내 캐릭터일 경우만)
			if (BrickPS->GetBrickPlayerID() == MyPlayerId && Image_Arrow)
			{
				if (UCanvasPanelSlot* ArrowSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot))
				{
					const float ArrowOffsetX = 30.f;
					const FVector2D ArrowPos = Pos + FVector2D(ArrowOffsetX, 0.f);
					ArrowSlot->SetPosition(ArrowPos);
				}
			}
		}
	}
}

