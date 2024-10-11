// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGSpawnUnitAction.h"
#include "Entities/Actions/RGActionsUtility.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGSpawnUnitAction, All, All);

URGSpawnUnitAction::URGSpawnUnitAction()
	: UnitClass(nullptr), TargetBuilding(nullptr), PlayerPawn(nullptr)
{
}

void URGSpawnUnitAction::InitializeAction(TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* InPlayerPawn)
{
	UnitClass = InUnitClass;
	TargetBuilding = InTargetBuilding;
	PlayerPawn = InPlayerPawn;
}

void URGSpawnUnitAction::Execute_Implementation()
{
	if (!UnitClass || !TargetBuilding || !PlayerPawn)
	{
		UE_LOG(LogRGSpawnUnitAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if(ActionData.ActionName == NAME_None)
	{
		UE_LOG(LogRGSpawnUnitAction, Warning, TEXT("[Execute] It is necessary to set ActionData before execution."));
		return;
	}

	if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData.ActionWoodCost))
	{
		TargetBuilding->AddUnitToSpawnQueue(UnitClass, ActionData.TimeToProduce);
		PlayerPawn->AddPlayerResources(-ActionData.ActionWoodCost);
	}
}