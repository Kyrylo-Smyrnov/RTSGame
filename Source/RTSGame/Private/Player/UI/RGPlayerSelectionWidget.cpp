// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGPlayerSelectionWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Entities/RGBuildingBase.h"
#include "Entities/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerSelectionWidget, All, All);

void URGPlayerSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		UE_LOG(LogRGPlayerSelectionWidget, Warning, TEXT("[NativeConstruct] PlayerController is nullptr."));
		return;
	}

	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGPlayerSelectionWidget, Warning, TEXT("[NativeConstruct] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->OnSelectedEntitiesChanged.AddUObject(this, &URGPlayerSelectionWidget::UpdateWidget);
}

void URGPlayerSelectionWidget::UpdateWidget(TArray<AActor*> SelectedEntities)
{
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

void URGPlayerSelectionWidget::InitializeWidget()
{
	if (!SelectionGrid)
	{
		UE_LOG(LogRGPlayerSelectionWidget, Warning, TEXT("[InitializeWidget] SelectionGrid is nullptr."));
		return;
	}

	const int32 ROWS = 2;
	const int32 COLS = 8;
	const float ICON_MARGIN = 5.0f;
	const FVector2D ICON_SIZE(75.0f, 75.0f);

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
				GridSlot->SetPadding(FMargin(ICON_MARGIN));
		}
	}
}