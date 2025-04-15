// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/DropAreaWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Trap/TrapBase.h"
#include "Network_Structure/BrickGamePlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"



bool UDropAreaWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation) return false;

    if (TSubclassOf<ATrapBase> TrapClass = Cast<UClass>(InOperation->Payload))
    {
        if (ABrickGamePlayerController* PC = Cast<ABrickGamePlayerController>(GetOwningPlayer()))
        {
            FVector2D ScreenPos = InDragDropEvent.GetScreenSpacePosition();
            float Scale = UWidgetLayoutLibrary::GetViewportScale(GetOwningPlayer());
            FVector2D ScaledScreenPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
            ScaledScreenPos *= Scale;
            PC->HandleTrapDrop(ScaledScreenPos, TrapClass);
            return true;
        }
    }

    return false;
}
