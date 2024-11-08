// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/SpawnUnitAction.h"
#include "Entities/Actions/ActionsUtility.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "GameFramework/Character.h"

void USpawnUnitAction::InitializeAction(const TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* InPlayerPawn)
{
	UnitClass = InUnitClass;
	TargetBuilding = InTargetBuilding;
	PlayerPawn = InPlayerPawn;
}

void USpawnUnitAction::Execute()
{
	if (!UnitClass || !TargetBuilding || !PlayerPawn || ActionData.ActionName == NAME_None)
	{
		return;
	}

	if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData.ActionWoodCost))
	{
		TargetBuilding->AddUnitToSpawnQueue(UnitClass, ActionData.TimeToProduce);
		PlayerPawn->AddPlayerResources(-ActionData.ActionWoodCost);
	}
}

void USpawnUnitAction::Cancel()
{
}