// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/TrapButtonWidget.h"
#include "InGame/TrapSetPointUserWidget.h"
#include "Trap/TrapBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

FReply UTrapButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return EventReply.NativeReply;
}

void UTrapButtonWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragOp = NewObject<UDragDropOperation>(this);
	DragOp->Payload = TrapClassToSpawn;
	DragOp->DefaultDragVisual = CreateWidget<UTrapSetPointUserWidget>(GetWorld(), TrapSetIcon);
	DragOp->Pivot = EDragPivot::CenterCenter;
	OutOperation = DragOp;
}
