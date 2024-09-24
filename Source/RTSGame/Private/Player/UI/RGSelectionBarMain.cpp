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
	if (SelectedBuilding)
		SelectedBuilding->OnSpawnQueueChanged.RemoveAll(this);

	if (SelectedEntities.Num() == 1)
	{
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(SelectedEntities[0]))
		{
			SelectedBuilding = CastedBuilding;
			SelectedBuilding->OnSpawnQueueChanged.AddUObject(this, &URGSelectionBarMain::HandleSpawnQueueChange);

			if (CastedBuilding->GetSpawnQueue().Num() != 0 || CastedBuilding->GetIsConstructing())
			{
				CurrentState = SelectionBarQueueState;
				SelectionBarSquad->SetVisibility(ESlateVisibility::Hidden);
				SelectionBarQueue->SetVisibility(ESlateVisibility::Visible);
				SelectionBarQueue->UpdateWidget(SelectedBuilding);
			}
		}
	}
	else
	{
		CurrentState = SelectionBarSquadState;
		SelectionBarQueue->SetVisibility(ESlateVisibility::Hidden);
		SelectionBarSquad->SetVisibility(ESlateVisibility::Visible);
		SelectionBarSquad->UpdateWidget(SelectedEntities);
	}
}

void URGSelectionBarMain::HandleSpawnQueueChange(TArray<FSpawnQueueEntry>& SpawnQueue)
{
	if (SpawnQueue.Num() == 0)
	{
		if (CurrentState == SelectionBarQueueState)
		{
			CurrentState = SelectionBarStatState;
			SelectionBarQueue->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (CurrentState != SelectionBarQueueState)
	{
		CurrentState = SelectionBarQueueState;
		SelectionBarSquad->SetVisibility(ESlateVisibility::Hidden);
		SelectionBarQueue->SetVisibility(ESlateVisibility::Visible);
		SelectionBarQueue->UpdateWidget(SelectedBuilding);
	}
}