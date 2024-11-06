// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGActionGridWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Components/EntityHandlerComponent.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"

void URGActionGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();

	const ARGPlayerController* PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		return;
	}

	EntityHandler = Cast<ARGPlayerPawn>(PlayerController->GetPawn())->GetEntityHandler();
	if (EntityHandler)
	{
		EntityHandler->OnMostImportantEntityChanged.AddUObject(this, &URGActionGridWidget::UpdateWidget);
	}
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
			Action.Value = nullptr;
			ActionIcons[Index]->SetVisibility(ESlateVisibility::Hidden);

			Index++;
		}
	}
	else
	{
		TArray<UBaseAction*> AvailableActions;
		if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(MostImportantEntity))
		{
			AvailableActions = CastedUnit->GetAvailableActions();
		}
		else if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(MostImportantEntity))
		{
			AvailableActions = CastedBuilding->GetAvailableActions();
		}

		int32 Index = 0;
		for (auto& Action : ActionButtons)
		{
			if (Index < ActionButtons.Num() && Index < AvailableActions.Num())
			{
				Action.Key->SetToolTipText(AvailableActions[Index]->GetActionData().ActionTooltip);
				Action.Key->SetIsEnabled(true);
				Action.Value = AvailableActions[Index];
				ActionIcons[Index]->SetBrushFromTexture(AvailableActions[Index]->GetActionData().ActionIcon, false);
				ActionIcons[Index]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Action.Key->SetToolTipText(FText::FromString(""));
				Action.Key->SetIsEnabled(false);
				Action.Value = nullptr;
				ActionIcons[Index]->SetVisibility(ESlateVisibility::Hidden);
			}

			Index++;
		}
	}
	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(MostImportantEntity))
	{
		if (CastedBuilding->GetIsConstructing())
		{
			for (auto& Action : ActionButtons)
			{
				Action.Key->SetIsEnabled(false);
			}
		}
	}
}

void URGActionGridWidget::InitializeWidget()
{
	ActionButtons.Add(ActionButton_0, nullptr);
	ActionButton_0->SetIsEnabled(false);
	ActionButton_0->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_0);
	ActionButtons.Add(ActionButton_1, nullptr);
	ActionButton_1->SetIsEnabled(false);
	ActionButton_1->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_1);
	ActionButtons.Add(ActionButton_2, nullptr);
	ActionButton_2->SetIsEnabled(false);
	ActionButton_2->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_2);
	ActionButtons.Add(ActionButton_3, nullptr);
	ActionButton_3->SetIsEnabled(false);
	ActionButton_3->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_3);
	ActionButtons.Add(ActionButton_4, nullptr);
	ActionButton_4->SetIsEnabled(false);
	ActionButton_4->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_4);
	ActionButtons.Add(ActionButton_5, nullptr);
	ActionButton_5->SetIsEnabled(false);
	ActionButton_5->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_5);
	ActionButtons.Add(ActionButton_6, nullptr);
	ActionButton_6->SetIsEnabled(false);
	ActionButton_6->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_6);
	ActionButtons.Add(ActionButton_7, nullptr);
	ActionButton_7->SetIsEnabled(false);
	ActionButton_7->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_7);
	ActionButtons.Add(ActionButton_8, nullptr);
	ActionButton_8->SetIsEnabled(false);
	ActionButton_8->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_8);
	ActionButtons.Add(ActionButton_9, nullptr);
	ActionButton_9->SetIsEnabled(false);
	ActionButton_9->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_9);
	ActionButtons.Add(ActionButton_10, nullptr);
	ActionButton_10->SetIsEnabled(false);
	ActionButton_10->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_10);
	ActionButtons.Add(ActionButton_11, nullptr);
	ActionButton_11->SetIsEnabled(false);
	ActionButton_11->OnClicked.AddDynamic(this, &URGActionGridWidget::HandleButtonClick);
	ActionIcons.Add(ActionImage_11);
}

void URGActionGridWidget::HandleButtonClick()
{
	for (const auto& ActionButton : ActionButtons)
	{
		if (ActionButton.Key->IsHovered() && ActionButton.Key->GetIsEnabled())
		{
			if (ActionButton.Value)
			{
				if (ActionButton.Value->GetActionData().TargetType != EActionTargetType::None)
				{
					EntityHandler->SetAwaitingAction(ActionButton.Value);
				}
				else
				{
					EntityHandler->ExecuteAction(ActionButton.Value);
				}

				return;
			}
		}
	}
}