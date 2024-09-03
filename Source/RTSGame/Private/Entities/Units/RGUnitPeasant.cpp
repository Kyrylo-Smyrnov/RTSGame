// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"
#include "Entities/Actions.h"
#include "Entities/Buildings/RGBuildingTownHall.h"

DEFINE_LOG_CATEGORY_STATIC(LogUnitPeasant, All, All);

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
	Super::PerformAction_Implementation(ActionName);

	FActorSpawnParameters BuildParameters;
	BuildParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ActionName == "BuildTownHall")
	{
		ARGBuildingTownHall* SpawnedTownHall =
			GetWorld()->SpawnActor<ARGBuildingTownHall>(BuildingTownHallBlueprintClass, BuildParameters);
		SpawnedTownHall->SetBuildingPlacementMaterial(true);
		
		return;
	}

	IActionable::PerformAction_Implementation(ActionName);
}
