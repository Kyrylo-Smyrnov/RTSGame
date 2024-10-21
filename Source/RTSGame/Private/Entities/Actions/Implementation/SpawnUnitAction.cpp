// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/SpawnUnitAction.h"
#include "Entities/Actions/ActionsUtility.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGSpawnUnitAction, All, All);

USpawnUnitAction::USpawnUnitAction()
	: UnitClass(nullptr), TargetBuilding(nullptr), PlayerPawn(nullptr)
{
}

void USpawnUnitAction::InitializeAction(TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* InPlayerPawn)
{
	UnitClass = InUnitClass;
	TargetBuilding = InTargetBuilding;
	PlayerPawn = InPlayerPawn;
}

void USpawnUnitAction::Execute_Implementation()
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