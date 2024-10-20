// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"
#include "Player/UI/RGActionGridWidget.h"
#include "Player/UI/RGPlayerResourcesWidget.h"
#include "Player/UI/RGSelectionBarMain.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerHud, All, All);

void ARGPlayerHUD::DrawHUD()
{
	if (bIsSelectionBoxDrawn)
	{
		if (PlayerController)
			PlayerController->GetMousePosition(SelectionBoxEndPoint.X, SelectionBoxEndPoint.Y);

		if (FVector2D::Distance(SelectionBoxStartPoint, SelectionBoxEndPoint) > 10.0f)
			DrawSelectionBox();
	}
}

URGActionGridWidget* ARGPlayerHUD::GetActionGridWidget() const
{
	return ActionGridWidget;
}

void ARGPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->LeftMouseButtonInputPressed.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputPressed);
		PlayerController->LeftMouseButtonInputReleased.AddUObject(this, &ARGPlayerHUD::HandleLeftMouseButtonInputReleased);
	}
	else
	{
		UE_LOG(LogRGPlayerHud, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));
		return;
	}

	if (ActionGridWidgetClass)
	{
		ActionGridWidget = CreateWidget<URGActionGridWidget>(PlayerController, ActionGridWidgetClass);
		if (ActionGridWidget)
			ActionGridWidget->AddToViewport();
	}
	if (ResourceWidgetClass)
	{
		URGPlayerResourcesWidget* ResourcesWidget = CreateWidget<URGPlayerResourcesWidget>(PlayerController, ResourceWidgetClass);
		if (ResourcesWidget)
			ResourcesWidget->AddToViewport();
	}
	if (SelectionBarMainClass)
	{
		URGSelectionBarMain* SelectionBarMain = CreateWidget<URGSelectionBarMain>(PlayerController, SelectionBarMainClass);
		if (SelectionBarMain)
			SelectionBarMain->AddToViewport();
	}
}

void ARGPlayerHUD::HandleLeftMouseButtonInputPressed()
{
	PlayerController->GetMousePosition(SelectionBoxStartPoint.X, SelectionBoxStartPoint.Y);
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

	GetActorsInSelectionRectangle(APawn::StaticClass(), SelectionBoxStartPoint, SelectionBoxEndPoint, SelectedEntities, false, false);
	SelectedEntities.RemoveAll([](AActor* Actor) { return Actor->IsA(ARGPlayerPawn::StaticClass()); });
}
