// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/ConstructBuildingAction.h"
#include "Entities/Actions/ActionsUtility.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Player/RGPlayerPawn.h"

UConstructBuildingAction::UConstructBuildingAction() : BuildingClass(nullptr), PlayerPawn(nullptr)
{
}

void UConstructBuildingAction::InitializeAction(TSubclassOf<AActor> InBuildingClass, ARGPlayerPawn* InPlayerPawn)
{
	BuildingClass = InBuildingClass;
	PlayerPawn = InPlayerPawn;
}

void UConstructBuildingAction::Execute()
{
	if (!BuildingClass || !PlayerPawn || ActionData.ActionName == NAME_None)
	{
		return;
	}

	FActorSpawnParameters ConstructParameters;
	ConstructParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData.ActionWoodCost))
	{
		ARGBuildingBase* SpawnedBuilding = PlayerPawn->GetWorld()->SpawnActor<ARGBuildingBase>(BuildingClass, ConstructParameters);
		if (SpawnedBuilding)
		{
			SpawnedBuilding->SetTimeToConstruct(ActionData.TimeToProduce);
			SpawnedBuilding->SetBuildingPlacementMaterial(true);
		}

		PlayerPawn->AddPlayerResources(-ActionData.ActionWoodCost);
	}
}