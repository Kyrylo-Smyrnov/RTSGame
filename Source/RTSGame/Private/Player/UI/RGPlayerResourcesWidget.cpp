// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGPlayerResourcesWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerResourcesWidget, All, All);

void URGPlayerResourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();

	ARGPlayerController* PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		UE_LOG(LogRGPlayerResourcesWidget, Warning, TEXT("[NativeConstruct] PlayerController is nullptr."));
		return;
	}

	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGPlayerResourcesWidget, Warning, TEXT("[NativeConstruct] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->OnPlayerResourcesChanged.AddUObject(this, &URGPlayerResourcesWidget::UpdateWidget);
}

void URGPlayerResourcesWidget::UpdateWidget(int32 WoodAmount)
{
	if (!WoodCountText)
	{
		UE_LOG(LogRGPlayerResourcesWidget, Warning, TEXT("[UpdateWidget] WoodCountText is nullptr."));
		return;
	}

	WoodCountText->SetText(FText::AsNumber(WoodAmount));
}

void URGPlayerResourcesWidget::InitializeWidget()
{
	if (!ResourcesHorizontalBox)
		return;

	const FVector2D ICON_SIZE(50.0f, 50.0f);
	const float PADDING = 10.0f;

	WoodIcon = NewObject<UImage>(this);
	WoodIcon->SetVisibility(ESlateVisibility::Visible);
	WoodIcon->SetBrushSize(ICON_SIZE);
	WoodIcon->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Player/Resources/T_IconPlayerWood")));

	WoodCountText = NewObject<UTextBlock>(this);
	WoodCountText->SetVisibility(ESlateVisibility::Visible);
	WoodCountText->SetText(FText::FromString("0"));

	USizeBox* IconSizeBox = NewObject<USizeBox>(this);
	IconSizeBox->SetWidthOverride(ICON_SIZE.X);
	IconSizeBox->SetHeightOverride(ICON_SIZE.Y);
	IconSizeBox->AddChild(WoodIcon);

	ResourcesHorizontalBox->AddChildToHorizontalBox(IconSizeBox);
	ResourcesHorizontalBox->AddChildToHorizontalBox(WoodCountText);
}
