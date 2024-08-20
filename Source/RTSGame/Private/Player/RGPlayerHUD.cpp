// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "RGPlayerController.h"

ARGPlayerHUD::ARGPlayerHUD()
{
}

void ARGPlayerHUD::DrawHUD()
{
	if(bIsSelectionBoxDrawn)
	{
		PlayerController->GetMousePosition(SelectionBoxEndPoint.X, SelectionBoxEndPoint.Y);
		DrawSelectionBox();
	}
}

void ARGPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->LeftMouseButtonInputPressed.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputPressed);
	PlayerController->LeftMouseButtonInputReleased.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputReleased);
}

void ARGPlayerHUD::HandleLeftMouseButtonInputPressed(FVector2D MousePosition)
{
	SelectionBoxStartPoint = MousePosition;
	bIsSelectionBoxDrawn = true;
}

void ARGPlayerHUD::HandleLeftMouseButtonInputReleased()
{
	bIsSelectionBoxDrawn = false;
}

void ARGPlayerHUD::DrawSelectionBox()
{
	DrawLine(SelectionBoxStartPoint.X, SelectionBoxStartPoint.Y, SelectionBoxEndPoint.X, SelectionBoxStartPoint.Y,
			 SELECTION_BOX_OUTLINE_COLOR, SELECTION_BOX_OUTLINE_THICKNESS);
	DrawLine(SelectionBoxStartPoint.X, SelectionBoxStartPoint.Y, SelectionBoxStartPoint.X, SelectionBoxEndPoint.Y,
			 SELECTION_BOX_OUTLINE_COLOR, SELECTION_BOX_OUTLINE_THICKNESS);
	DrawLine(SelectionBoxStartPoint.X, SelectionBoxEndPoint.Y, SelectionBoxEndPoint.X, SelectionBoxEndPoint.Y,
			 SELECTION_BOX_OUTLINE_COLOR, SELECTION_BOX_OUTLINE_THICKNESS);
	DrawLine(SelectionBoxEndPoint.X, SelectionBoxEndPoint.Y, SelectionBoxEndPoint.X, SelectionBoxStartPoint.Y,
			 SELECTION_BOX_OUTLINE_COLOR, SELECTION_BOX_OUTLINE_THICKNESS);
}
