// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGSelectionBarSquad.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Entities/RGBuildingBase.h"
#include "Entities/RGUnitBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogURGSelectionBarSquad, All, All);

void URGSelectionBarSquad::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}

void URGSelectionBarSquad::UpdateWidget(TArray<AActor*> SelectedEntities)
{
	if(GetVisibility() == ESlateVisibility::Hidden)
		return;
	
	for (int32 i = 0; i < EntitiesIcons.Num(); ++i)
	{
		if (i < SelectedEntities.Num())
		{
			ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(SelectedEntities[i]);
			if (CastedUnit)
				EntitiesIcons[i]->SetBrushFromTexture(CastedUnit->GetSelectionIcon());
			ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(SelectedEntities[i]);
			if (CastedBuilding)
				EntitiesIcons[i]->SetBrushFromTexture(CastedBuilding->GetSelectionIcon());

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
	if (!SelectionGrid)
	{
		UE_LOG(LogURGSelectionBarSquad, Warning, TEXT("[InitializeWidget] SelectionGrid is nullptr."));
		return;
	}

	const int32 ROWS = 2;
	const int32 COLS = 8;
	const float ICON_PADDING = 1.0f;
	const float ICON_MARGIN = 20.0f;
	const FVector2D ICON_SIZE(60.0f, 60.0f);

	SelectionGrid->SetRenderTranslation(
		FVector2D((-ICON_SIZE.X * COLS - ICON_MARGIN) / 2, -ICON_SIZE.Y * ROWS - ICON_MARGIN));

	for (int32 Row = 0; Row < ROWS; ++Row)
	{
		for (int32 Col = 0; Col < COLS; ++Col)
		{
			UImage* EntityIcon = NewObject<UImage>(this);
			EntityIcon->SetVisibility(ESlateVisibility::Hidden);
			EntityIcon->SetBrushSize(ICON_SIZE);
			EntitiesIcons.Add(EntityIcon);

			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(ICON_SIZE.X);
			SizeBox->SetHeightOverride(ICON_SIZE.Y);
			SizeBox->AddChild(EntityIcon);

			UGridSlot* GridSlot = Cast<UGridSlot>(SelectionGrid->AddChildToGrid(SizeBox, Row, Col));

			if (GridSlot)
				GridSlot->SetPadding(FMargin(ICON_PADDING));
		}
	}
}