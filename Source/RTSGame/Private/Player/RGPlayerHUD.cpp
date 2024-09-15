// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "Player/UI/RGActionGridWidget.h"
#include "RGPlayerController.h"

ARGPlayerHUD::ARGPlayerHUD()
{
}

void ARGPlayerHUD::DrawHUD()
{
	if (bIsSelectionBoxDrawn)
	{
		PlayerController->GetMousePosition(SelectionBoxEndPoint.X, SelectionBoxEndPoint.Y);

		if (FVector2D::Distance(SelectionBoxStartPoint, SelectionBoxEndPoint) > 10.0f)
			DrawSelectionBox();
	}
}

void ARGPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->LeftMouseButtonInputPressed.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputPressed);
	PlayerController->LeftMouseButtonInputReleased.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputReleased);

	if (ActionGridWidgetClass)
	{
		URGActionGridWidget* ActionGridWidget = CreateWidget<URGActionGridWidget>(PlayerController, ActionGridWidgetClass);
		if (ActionGridWidget)
			ActionGridWidget->AddToViewport();
	}
}

void ARGPlayerHUD::HandleLeftMouseButtonInputPressed(FVector2D MousePosition)
{
	SelectionBoxStartPoint = MousePosition;
	bIsSelectionBoxDrawn = true;
}

void ARGPlayerHUD::HandleLeftMouseButtonInputReleased()
{
	bIsSelectionBoxDrawn = false;

	if (bIsSelectionBox)
	{
		ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
		if (PlayerPawn)
		{
			PlayerPawn->ClearSelectedEntities();
			PlayerPawn->AddEntitiesToSelected(SelectedEntities);

			bIsSelectionBox = false;
		}
	}
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

	bIsSelectionBox = true;

	GetActorsInSelectionRectangle(APawn::StaticClass(), SelectionBoxStartPoint, SelectionBoxEndPoint, SelectedEntities,
								  false, false);

	SelectedEntities.RemoveAll([](AActor* Actor) { return Actor->IsA(ARGPlayerPawn::StaticClass()); });
}
