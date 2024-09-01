// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"
#include "Entities/Actions.h"

ARGBuildingTownHall::ARGBuildingTownHall() : ARGBuildingBase()
{
	this->BuildingImportance = EFEntitiesImportance::TownHall;
}

void ARGBuildingTownHall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARGBuildingTownHall::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FActionData> ARGBuildingTownHall::GetAvailableActions_Implementation() const
{
	TArray<FActionData> TownHallActions;

	TownHallActions.Add(BuildingActions::TownHall_BuildPeasant);

	return TownHallActions;
}

void ARGBuildingTownHall::PerformAction_Implementation(const FName& ActionName)
{
	IActionable::PerformAction_Implementation(ActionName);
}
