// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGSelectionBarQueue.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Entities/RGBuildingBase.h"
#include "Entities/RGUnitBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGSelectionBarQueue, All, All);

void URGSelectionBarQueue::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}

void URGSelectionBarQueue::UpdateWidget(ARGBuildingBase* Building)
{
	if (CurrentBuilding)
	{
		CurrentBuilding->OnSpawnQueueChanged.RemoveAll(this);
		CurrentBuilding->OnSpawnProgressChanged.RemoveAll(this);
		CurrentBuilding->OnConstructionProgressChanged.RemoveAll(this);
	}

	CurrentBuilding = Building;

	CurrentBuilding->OnSpawnQueueChanged.AddUObject(this, &URGSelectionBarQueue::HandleSpawnQueueChange);
	CurrentBuilding->OnSpawnProgressChanged.AddUObject(this, &URGSelectionBarQueue::HandleSpawnProgressChange);

	if (CurrentBuilding->GetIsConstructing())
	{
		CurrentBuilding->OnConstructionProgressChanged.AddUObject(this, &URGSelectionBarQueue::HandleSpawnProgressChange);
		HandleBuildingConstruction();
	}
	else
	{
		HandleSpawnQueueChange(CurrentBuilding->GetSpawnQueue());
	}
}

void URGSelectionBarQueue::InitializeWidget()
{
	if (!MainVerticalBox)
	{
		UE_LOG(LogRGSelectionBarQueue, Warning, TEXT("[InitializeWidget] MainVerticalBox is nullptr."));
		return;
	}

	const float ICON_PADDING = 1.0f;
	const float ICON_MARGIN = 20.0f;
	const FVector2D ICON_SIZE(60.0f, 60.0f);

	MainVerticalBox->SetRenderTranslation(
		FVector2D((-ICON_SIZE.X * 8 - ICON_MARGIN) / 2, -ICON_SIZE.Y * 2 - ICON_MARGIN));

	UHorizontalBox* CurrentUnitBox = NewObject<UHorizontalBox>(this);

	CurrentUnitIcon = NewObject<UImage>(this);
	CurrentUnitIcon->SetBrushSize(ICON_SIZE);
	CurrentUnitIcon->SetVisibility(ESlateVisibility::Hidden);

	UHorizontalBoxSlot* CurrentUnitImgSlot = CurrentUnitBox->AddChildToHorizontalBox(CurrentUnitIcon);
	if (CurrentUnitImgSlot)
	{
		CurrentUnitImgSlot->SetPadding(FMargin(ICON_PADDING));
		CurrentUnitImgSlot->SetHorizontalAlignment(HAlign_Left);
	}

	SpawnProgressBar = NewObject<UProgressBar>(this);
	SpawnProgressBar->SetFillColorAndOpacity(FLinearColor::Green);
	SpawnProgressBar->SetPercent(0.0f);
	SpawnProgressBar->SetVisibility(ESlateVisibility::Hidden);

	USizeBox* ProgressBarSizeBox = NewObject<USizeBox>(this);
	ProgressBarSizeBox->SetWidthOverride(ICON_SIZE.X * 7 + ICON_PADDING * 7);
	ProgressBarSizeBox->AddChild(SpawnProgressBar);

	UHorizontalBoxSlot* ProgressBarSlot = Cast<UHorizontalBoxSlot>(CurrentUnitBox->AddChild(ProgressBarSizeBox));
	if (ProgressBarSlot)
	{
		ProgressBarSlot->SetPadding(FMargin(ICON_PADDING, 0.0f, 0.0f, 0.0f)); // Симметричные отступы
		ProgressBarSlot->SetHorizontalAlignment(HAlign_Fill);
	}

	MainVerticalBox->AddChildToVerticalBox(CurrentUnitBox);

	UHorizontalBox* UnitsInQueueBox = NewObject<UHorizontalBox>(this);

	for (int i = 0; i < 8; ++i)
	{
		UImage* UnitIcon = NewObject<UImage>(this);
		UnitIcon->SetBrushSize(ICON_SIZE);
		UnitIcon->SetVisibility(ESlateVisibility::Hidden);
		UnitsInQueueIcons.Add(UnitIcon);

		USizeBox* SizeBox = NewObject<USizeBox>(this);
		SizeBox->SetWidthOverride(ICON_SIZE.X);
		SizeBox->SetHeightOverride(ICON_SIZE.Y);
		SizeBox->AddChild(UnitIcon);

		UHorizontalBoxSlot* UnitImgSlot = UnitsInQueueBox->AddChildToHorizontalBox(SizeBox);
		if (UnitImgSlot)
		{
			UnitImgSlot->SetPadding(FMargin(ICON_PADDING));
			UnitImgSlot->SetHorizontalAlignment(HAlign_Center);
		}
	}

	MainVerticalBox->AddChildToVerticalBox(UnitsInQueueBox);
}

void URGSelectionBarQueue::HandleSpawnQueueChange(TArray<FSpawnQueueEntry>& SpawnQueue)
{
	if (SpawnQueue.Num() == 0)
	{
		SpawnProgressBar->SetPercent(0.0f);
		return;
	}

	SpawnProgressBar->SetVisibility(ESlateVisibility::Visible);
	
	ARGUnitBase* CurrentUnit = Cast<ARGUnitBase>(SpawnQueue[0].UnitClass.GetDefaultObject());
	if (CurrentUnit)
	{
		if (CurrentUnit)
		{
			CurrentUnitIcon->SetBrushFromTexture(CurrentUnit->GetSelectionIcon());
			CurrentUnitIcon->SetVisibility(ESlateVisibility::Visible);
		}

		for (int i = 0; i < UnitsInQueueIcons.Num(); ++i)
		{
			if (i + 1 < SpawnQueue.Num())
			{
				CurrentUnit = Cast<ARGUnitBase>(SpawnQueue[i + 1].UnitClass.GetDefaultObject());
				if (CurrentUnit)
				{
					UnitsInQueueIcons[i]->SetBrushFromTexture(CurrentUnit->GetSelectionIcon());
					UnitsInQueueIcons[i]->SetVisibility(ESlateVisibility::Visible);
				}
			}
			else
			{
				UnitsInQueueIcons[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void URGSelectionBarQueue::HandleBuildingConstruction()
{
	SpawnProgressBar->SetVisibility(ESlateVisibility::Visible);
	
	CurrentUnitIcon->SetBrushFromTexture(CurrentBuilding->GetSelectionIcon());
	CurrentUnitIcon->SetVisibility(ESlateVisibility::Visible);

	for(UImage* Icon : UnitsInQueueIcons)
		Icon->SetVisibility(ESlateVisibility::Hidden);
}

void URGSelectionBarQueue::HandleSpawnProgressChange(float Progress)
{
	if (SpawnProgressBar)
		SpawnProgressBar->SetPercent(Progress);
}