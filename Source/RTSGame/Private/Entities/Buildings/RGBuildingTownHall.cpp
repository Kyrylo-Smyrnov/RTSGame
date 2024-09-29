// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Actions/Actions.h"
#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Units/RGUnitPeasant.h"

DEFINE_LOG_CATEGORY_STATIC(LogBuildingTownHall, All, All);

ARGBuildingTownHall::ARGBuildingTownHall()
	: ARGBuildingBase()
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
	if (GetIsConstructing())
		return;

	TArray<FActionData> AvailableActions = GetAvailableActions_Implementation();
	FActionData* ActionData = AvailableActions.FindByPredicate([&](const FActionData& Action) { return Action.ActionName == ActionName; });

	if (ActionName == ACTION_BUILDPEASANT)
	{
		if (!UnitPeasantBlueprintClass)
		{
			UE_LOG(LogBuildingTownHall, Warning, TEXT("[PerformAction_Implementation] UnitPeasantBlueprintClass is nullptr."));
			return;
		}

		if (ActionsUtility::IsEnoughResourcesToBuild(PlayerPawn, ActionData->ActionWoodCost))
		{
			AddUnitToSpawnQueue(UnitPeasantBlueprintClass, ActionData->UnitSpawnTime);
			PlayerPawn->AddPlayerResources(-ActionData->ActionWoodCost);
		}
		else
		{
			UE_LOG(LogBuildingTownHall, Display, TEXT("There are not enough resources to perform the action %s"), *ActionData->ActionName.ToString());
		}
	}

	IActionable::PerformAction_Implementation(ActionName);
}

void ARGBuildingTownHall::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Super::HandleOnClicked(TouchedActor, ButtonPressed);
	TArray<AActor*> SelectedEntities = PlayerPawn->GetSelectedEntities();
	for (int i = 0; i < SelectedEntities.Num(); ++i)
	{
		if (ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(SelectedEntities[i]))
		{
			if (CastedPeasant->GetIsCarryingResources())
			{
				UBlackboardComponent* Blackboard = Cast<ARGUnitAIController>(CastedPeasant->GetController())->GetBlackboardComponent();
				Blackboard->SetValueAsEnum("UnitState", 2);
			}
		}
	}
}
