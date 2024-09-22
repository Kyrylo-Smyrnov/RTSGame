// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"
#include "Entities/Actions.h"
#include "Entities/Buildings/RGBuildingTownHall.h"
#include "Player/RGPlayerPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogUnitPeasant, All, All);

ARGUnitPeasant::ARGUnitPeasant()
	: ARGUnitBase(), CarryingWood(0)
{
	UnitImportance = EFEntitiesImportance::Peasant;
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

	TArray<FActionData> AvailableActions = GetAvailableActions_Implementation();
	FActionData* ActionData = AvailableActions.FindByPredicate([&](const FActionData& Action) { return Action.ActionName == ActionName; });

	if (ActionName == ACTION_BUILDTOWNHALL)
	{
		if (!BuildingTownHallBlueprintClass)
		{
			UE_LOG(LogUnitPeasant, Warning, TEXT("[PerformAction_Implementation] BuildingTownHallBlueprintClass is nullptr."));
			return;
		}

		if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData->ActionWoodCost))
		{
			ARGBuildingTownHall* SpawnedTownHall = GetWorld()->SpawnActor<ARGBuildingTownHall>(BuildingTownHallBlueprintClass, BuildParameters);
			if (SpawnedTownHall)
			{
				SpawnedTownHall->SetTimeToConstruct(ActionData->UnitSpawnTime);
				SpawnedTownHall->SetBuildingPlacementMaterial(true);
			}

			PlayerPawn->AddPlayerResources(-ActionData->ActionWoodCost);
		}
		else
		{
			UE_LOG(LogUnitPeasant, Display, TEXT("There are not enough resources to perform the action %s"), *ActionData->ActionName.ToString());
		}

		return;
	}

	IActionable::PerformAction_Implementation(ActionName);
}

void ARGUnitPeasant::AddCarryingWood(int32 Amount)
{
	CarryingWood += Amount;
}

void ARGUnitPeasant::PutCarryingResources()
{
	if (!PlayerPawn)
	{
		UE_LOG(LogUnitPeasant, Warning, TEXT("[PutCarryingResources] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->AddPlayerResources(CarryingWood);
	CarryingWood = 0;
}
