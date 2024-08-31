// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"
#include "Entities/ActionsTooltips.h"

ARGUnitPeasant::ARGUnitPeasant() : ARGUnitBase()
{
	this->UnitImportance = EFEntitiesImportance::Peasant;
}

void ARGUnitPeasant::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARGUnitPeasant::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FActionData> ARGUnitPeasant::GetAvailableActions_Implementation() const
{
	TArray<FActionData> PeasantActions;

	PeasantActions.Append(Super::GetAvailableActions_Implementation());

	FActionData BuildTownHall;
	BuildTownHall.ActionName = "BuildTownHall";
	BuildTownHall.ActionIcon =
		LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/Peasant/T_IconBuildTownHall"));
	BuildTownHall.ActionTooltip = Tooltips::Units::Peasant::BuildTownHall;
	PeasantActions.Add(BuildTownHall);

	return PeasantActions;
}

void ARGUnitPeasant::PerformAction_Implementation(const FName& ActionName)
{
	IActionable::PerformAction_Implementation(ActionName);
}
