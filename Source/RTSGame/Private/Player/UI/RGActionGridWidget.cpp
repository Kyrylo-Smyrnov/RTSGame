// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGActionGridWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
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
		for(int32 i = 0; i < ActionButtons.Num(); ++i)
		{
			ActionButtons[i]->SetVisibility(ESlateVisibility::Visible);
			ActionButtons[i]->SetIsEnabled(false);
			ActionIcons[i]->SetVisibility(ESlateVisibility::Hidden);
		}

	if (IActionable* ActionableEntity = Cast<IActionable>(MostImportantEntity))
	{
		TArray<FActionData> AvailableActions = ActionableEntity->GetAvailableActions_Implementation();

		for (int32 i = 0; i < AvailableActions.Num(); ++i)
		{
			if (i < AvailableActions.Num())
			{
				ActionButtons[i]->SetVisibility(ESlateVisibility::Visible);
				ActionButtons[i]->SetIsEnabled(true);
				ActionIcons[i]->SetVisibility(ESlateVisibility::Visible);
				ActionIcons[i]->SetBrushFromTexture(AvailableActions[i].ActionIcon);
			}
			else
			{
				ActionButtons[i]->SetVisibility(ESlateVisibility::Visible);
				ActionButtons[i]->SetIsEnabled(false);
			}
		}
	}
}

void URGActionGridWidget::InitializeGrid()
{
	if (!ActionGrid)
		return;

	for (int32 Row = 0; Row < 3; ++Row)
	{
		for (int32 Col = 0; Col < 4; ++Col)
		{
			UButton* ActionButton = NewObject<UButton>(this);
			UImage* ActionImage = NewObject<UImage>(this);

			ActionButton->AddChild(ActionImage);
			ActionGrid->AddChild(ActionButton);

			ActionButtons.Add(ActionButton);
			ActionIcons.Add(ActionImage);

			UGridSlot* GridSlot = Cast<UGridSlot>(ActionGrid->AddChildToGrid(ActionButton));

			if (GridSlot)
			{
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Col);
			}

			ActionButton->SetVisibility(ESlateVisibility::Visible);
			ActionButton->SetIsEnabled(false);
		}
	}
}