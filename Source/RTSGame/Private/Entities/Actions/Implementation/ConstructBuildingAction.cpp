// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/ConstructBuildingAction.h"
#include "Entities/Actions/ActionsUtility.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Player/RGPlayerPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGConstructBuildingAction, All, All);

UConstructBuildingAction::UConstructBuildingAction()
	: BuildingClass(nullptr), PlayerPawn(nullptr)
{
}

void UConstructBuildingAction::InitializeAction(TSubclassOf<AActor> InBuildingClass, ARGPlayerPawn* InPlayerPawn)
{
	BuildingClass = InBuildingClass;
	PlayerPawn = InPlayerPawn;
}

void UConstructBuildingAction::Execute_Implementation()
{
	if (!BuildingClass || !PlayerPawn)
	{
		UE_LOG(LogRGConstructBuildingAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if (ActionData.ActionName == NAME_None)
	{
		UE_LOG(LogRGConstructBuildingAction, Warning, TEXT("[Execute] It is necessary to set ActionData before execution."));
		return;
	}

	FActorSpawnParameters ConstructParameters;
	ConstructParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData.ActionWoodCost))
	{
		ARGBuildingBase* SpawnedBuilding = PlayerPawn->GetWorld()->SpawnActor<ARGBuildingBase>(BuildingClass, ConstructParameters);
		if(SpawnedBuilding)
		{
			SpawnedBuilding->SetTimeToConstruct(ActionData.TimeToProduce);
			SpawnedBuilding->SetBuildingPlacementMaterial(true);
		}

		PlayerPawn->AddPlayerResources(-ActionData.ActionWoodCost);
	}
}