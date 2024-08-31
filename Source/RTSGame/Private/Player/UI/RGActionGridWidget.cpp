// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGActionGridWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Entities/Actionable.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

void URGActionGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeGrid();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	PlayerPawn->OnSelectedEntitiesChanged.AddUObject(this, &URGActionGridWidget::UpdateActionButtons);
}

void URGActionGridWidget::UpdateActionButtons(AActor* MostImportantEntity)
{
	if (!MostImportantEntity)
	{
		for (int32 i = 0; i < ActionButtons.Num(); ++i)
		{
			ActionButtons[i]->SetToolTipText(FText::FromString(""));
			ActionButtons[i]->SetIsEnabled(false);
			ActionIcons[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (IActionable* ActionableEntity = Cast<IActionable>(MostImportantEntity))
	{
		TArray<FActionData> AvailableActions = ActionableEntity->GetAvailableActions_Implementation();

		for (int32 i = 0; i < ActionButtons.Num(); ++i)
		{
			if (i < AvailableActions.Num())
			{
				ActionButtons[i]->SetToolTipText(AvailableActions[i].ActionTooltip);
				ActionButtons[i]->SetIsEnabled(true);

				ActionIcons[i]->SetBrushFromTexture(AvailableActions[i].ActionIcon, false);
				ActionIcons[i]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ActionButtons[i]->SetToolTipText(FText::FromString(""));
				ActionButtons[i]->SetIsEnabled(false);
				ActionIcons[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void URGActionGridWidget::InitializeGrid()
{
	if (!ActionGrid)
		return;

	const int32 ROWS = 3;
	const int32 COLS = 4;
	const float BUTTON_PADDING = 1.0f;
	const float BUTTON_MARGIN = 20.0f;
	const FVector2D BUTTON_SIZE(75.0f, 75.0f);

	ActionGrid->SetRenderTranslation(
		FVector2D(-BUTTON_SIZE.X * COLS - BUTTON_MARGIN, -BUTTON_SIZE.Y * ROWS - BUTTON_MARGIN));

	for (int32 Row = 0; Row < ROWS; ++Row)
	{
		for (int32 Col = 0; Col < COLS; ++Col)
		{
			UImage* ActionIcon = NewObject<UImage>(this);
			ActionIcon->SetVisibility(ESlateVisibility::Hidden);
			ActionIcon->SetBrushSize(BUTTON_SIZE);
			ActionIcons.Add(ActionIcon);

			UButton* ActionButton = NewObject<UButton>(this);
			ActionButton->SetVisibility(ESlateVisibility::Visible);
			ActionButton->SetIsEnabled(false);
			ActionButton->AddChild(ActionIcon);
			ActionButtons.Add(ActionButton);

			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(BUTTON_SIZE.X);
			SizeBox->SetHeightOverride(BUTTON_SIZE.Y);
			SizeBox->AddChild(ActionButton);

			ActionGrid->AddChild(SizeBox);

			UGridSlot* GridSlot = Cast<UGridSlot>(ActionGrid->AddChildToGrid(SizeBox, Row, Col));

			if (GridSlot)
				GridSlot->SetPadding(FMargin(BUTTON_PADDING));
		}
	}
}