// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/UI/RGSelectionBarMain.h"
#include "Entities/RGBuildingBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "Player/UI/RGSelectionBarQueue.h"
#include "Player/UI/RGSelectionBarSquad.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGSelectionBarMain, All, All);

void URGSelectionBarMain::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		UE_LOG(LogRGSelectionBarMain, Warning, TEXT("[NativeConstruct] PlayerController is nullptr."));
		return;
	}

	ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGSelectionBarMain, Warning, TEXT("[NativeConstruct] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->OnSelectedEntitiesChanged.AddUObject(this, &URGSelectionBarMain::UpdateWidget);
}

void URGSelectionBarMain::UpdateWidget(TArray<AActor*> SelectedEntities)
{
	if (SelectedEntities.Num() == 1)
	{
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(SelectedEntities[0]))
			if (CastedBuilding->GetSpawnQueue().Num() != 0)
			{
				SelectionBarSquad->SetVisibility(ESlateVisibility::Hidden);
				SelectionBarQueue->SetVisibility(ESlateVisibility::Visible);
				SelectionBarQueue->UpdateWidget(CastedBuilding);
			}
	}
	else
	{
		SelectionBarQueue->SetVisibility(ESlateVisibility::Hidden);
		SelectionBarSquad->SetVisibility(ESlateVisibility::Visible);
		SelectionBarSquad->UpdateWidget(SelectedEntities);
	}
}