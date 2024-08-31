// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"
#include "Entities/ActionsTooltips.h"

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

	FActionData BuildPeasant;
	BuildPeasant.ActionName = "BuildPeasant";
	BuildPeasant.ActionIcon =
		LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Buildings/TownHall/T_IconBuildPeasant"));
	BuildPeasant.ActionTooltip = Tooltips::Buildings::TownHall::BuildPeasantTooltip;
	TownHallActions.Add(BuildPeasant);

	return TownHallActions;
}

void ARGBuildingTownHall::PerformAction_Implementation(const FName& ActionName)
{
	IActionable::PerformAction_Implementation(ActionName);
}
