// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"
#include "Entities/Actions.h"

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
	PeasantActions.Add(UnitActions::Peasant_BuildTownHall);

	return PeasantActions;
}

void ARGUnitPeasant::PerformAction_Implementation(const FName& ActionName)
{
	IActionable::PerformAction_Implementation(ActionName);
}
