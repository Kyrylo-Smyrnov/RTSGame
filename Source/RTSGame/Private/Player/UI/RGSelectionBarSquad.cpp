// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGSelectionBarSquad.h"
#include "Components/Image.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitBase.h"

void URGSelectionBarSquad::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}

void URGSelectionBarSquad::UpdateWidget(TArray<AActor*> SelectedEntities)
{
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		return;
	}

	for (int32 i = 0; i < EntitiesIcons.Num(); ++i)
	{
		if (i < SelectedEntities.Num())
		{
			if (const ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(SelectedEntities[i]))
			{
				EntitiesIcons[i]->SetBrushFromTexture(CastedUnit->GetSelectionIcon());
			}
			else if (const ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(SelectedEntities[i]))
			{
				EntitiesIcons[i]->SetBrushFromTexture(CastedBuilding->GetSelectionIcon());
			}
			
			EntitiesIcons[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			EntitiesIcons[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URGSelectionBarSquad::InitializeWidget()
{
	EntityIcon_0->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_0);
	EntityIcon_1->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_1);
	EntityIcon_2->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_2);
	EntityIcon_3->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_3);
	EntityIcon_4->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_4);
	EntityIcon_5->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_5);
	EntityIcon_6->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_6);
	EntityIcon_7->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_7);
	EntityIcon_8->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_8);
	EntityIcon_9->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_9);
	EntityIcon_10->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_10);
	EntityIcon_11->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_11);
	EntityIcon_12->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_12);
	EntityIcon_13->SetVisibility(ESlateVisibility::Hidden);
	EntitiesIcons.Add(EntityIcon_13);
}