// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGSelectionBarQueue.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Entities/RGBuildingBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGSelectionBarQueue, All, All);

void URGSelectionBarQueue::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}

void URGSelectionBarQueue::UpdateWidget(ARGBuildingBase* Building)
{
	TArray<FSpawnQueueEntry> SpawnQueue = Building->GetSpawnQueue();
	if(SpawnQueue.Num() != 0)
	{
		CurrentUnitIcon->SetVisibility(ESlateVisibility::Visible);
		SpawnProgressBar->SetVisibility(ESlateVisibility::Visible);
		for(int i = 0; i < UnitsInQueueIcons.Num(); ++i)
			UnitsInQueueIcons[i]->SetVisibility(ESlateVisibility::Visible);
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
		CurrentUnitImgSlot->SetPadding(ICON_PADDING);

	SpawnProgressBar = NewObject<UProgressBar>(this);
	SpawnProgressBar->SetPercent(50.0f);
	SpawnProgressBar->SetVisibility(ESlateVisibility::Hidden);
	UHorizontalBoxSlot* ProgressBarSlot = Cast<UHorizontalBoxSlot>(CurrentUnitBox->AddChild(SpawnProgressBar));
	if (ProgressBarSlot)
		ProgressBarSlot->SetPadding(ICON_PADDING);

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
			UnitImgSlot->SetPadding(ICON_PADDING);
	}

	MainVerticalBox->AddChildToVerticalBox(UnitsInQueueBox);
}