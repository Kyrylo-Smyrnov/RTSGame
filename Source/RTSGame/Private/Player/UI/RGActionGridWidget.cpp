// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGActionGridWidget.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Entities/Actionable.h"
#include "Entities/Buildings/RGBuildingTownHall.h"
#include "Entities/Units/RGUnitPeasant.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGActionGridWidget, All, All);

void URGActionGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		UE_LOG(LogRGActionGridWidget, Warning, TEXT("[NativeConstruct] PlayerController is nullptr."));
		return;
	}

	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGActionGridWidget, Warning, TEXT("[NativeConstruct] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->OnMostImportantEntityChanged.AddUObject(this, &URGActionGridWidget::UpdateWidget);
}

void URGActionGridWidget::UpdateWidget(AActor* MostImportantEntity)
{
	if (!MostImportantEntity)
	{
		int32 Index = 0;
		for (auto& Action : ActionButtons)
		{
			Action.Key->SetToolTipText(FText::FromString(""));
			Action.Key->SetIsEnabled(false);
			Action.Value = "";
			ActionIcons[Index]->SetVisibility(ESlateVisibility::Hidden);

			Index++;
		}
	}
	else if (IActionable* ActionableEntity = Cast<IActionable>(MostImportantEntity))
	{
		TArray<FActionData> AvailableActions = ActionableEntity->GetAvailableActions_Implementation();

		int32 Index = 0;
		for (auto& Action : ActionButtons)
		{
			if (Index < ActionButtons.Num() && Index < AvailableActions.Num())
			{
				Action.Key->SetToolTipText(AvailableActions[Index].ActionTooltip);
				Action.Key->SetIsEnabled(true);
				Action.Value = AvailableActions[Index].ActionName;
				ActionIcons[Index]->SetBrushFromTexture(AvailableActions[Index].ActionIcon, false);
				ActionIcons[Index]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Action.Key->SetToolTipText(FText::FromString(""));
				Action.Key->SetIsEnabled(false);
				Action.Value = "";
				ActionIcons[Index]->SetVisibility(ESlateVisibility::Hidden);
			}

			Index++;
		}
	}

	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(MostImportantEntity))
	{
		if(CastedBuilding->GetIsConstructing())
		{
			for(auto& Action : ActionButtons)
			{
				Action.Key->SetIsEnabled(false);
			}
		}
	}
}

void URGActionGridWidget::InitializeWidget()
{
	if (!ActionGrid)
	{
		UE_LOG(LogRGActionGridWidget, Warning, TEXT("[InitializeWidget] ActionGrid is nullptr."));
		return;
	}

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
			ActionButton->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
			ActionButtons.Add(ActionButton, "");

			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(BUTTON_SIZE.X);
			SizeBox->SetHeightOverride(BUTTON_SIZE.Y);
			SizeBox->AddChild(ActionButton);

			UGridSlot* GridSlot = Cast<UGridSlot>(ActionGrid->AddChildToGrid(SizeBox, Row, Col));

			if (GridSlot)
				GridSlot->SetPadding(FMargin(BUTTON_PADDING));
		}
	}
}

void URGActionGridWidget::HandleButtonClick()
{
	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGActionGridWidget, Warning, TEXT("[HandleButtonClick] PlayerPawn is nullptr."))
		return;
	}

	AActor* MostImportanEntity = PlayerPawn->GetMostImportantEntity();
	if (!MostImportanEntity)
	{
		UE_LOG(LogRGActionGridWidget, Warning, TEXT("[HandleButtonClick] MostImportanEntity is nullptr."))
		return;
	}

	for (auto& ActionButton : ActionButtons)
	{
		if (ActionButton.Key->IsHovered() && ActionButton.Key->GetIsEnabled())
		{
			if (ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(MostImportanEntity))
				CastedPeasant->PerformAction_Implementation(ActionButton.Value);
			else if (ARGBuildingTownHall* CastedTownHall = Cast<ARGBuildingTownHall>(MostImportanEntity))
				CastedTownHall->PerformAction_Implementation(ActionButton.Value);
		}
	}
}