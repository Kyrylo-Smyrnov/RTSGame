// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"

#include "Entities/Actions/Implementation/CollectResourceAction.h"
#include "Entities/Actions/Implementation/ConstructBuildingAction.h"
#include "Entities/Buildings/RGBuildingTownHall.h"

DEFINE_LOG_CATEGORY_STATIC(LogUnitPeasant, All, All);

ARGUnitPeasant::ARGUnitPeasant()
	: ARGUnitBase(), CarryingWood(0)
{
	UnitImportance = EFEntitiesImportance::Peasant;
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

bool ARGUnitPeasant::GetIsCarryingResources() const
{
	return CarryingWood != 0;
}

void ARGUnitPeasant::BeginPlay()
{
	Super::BeginPlay();
}

void ARGUnitPeasant::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARGUnitPeasant::InitializeActions()
{
	Super::InitializeActions();
	
	UConstructBuildingAction* BuildTownHallAction = NewObject<UConstructBuildingAction>();
	FActionData BuildTownHallData = UnitActions::Peasant_BuildTownHall;
	BuildTownHallAction->InitializeAction(BuildingTownHallBlueprintClass, PlayerPawn);
	BuildTownHallData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/Peasant/T_IconBuildTownHall"));
	BuildTownHallAction->SetActionData(BuildTownHallData);

	UCollectResourceAction* CollectResourceAction = NewObject<UCollectResourceAction>();
	FActionData CollectResourceData = UnitActions::Peasant_CollectResource;
	CollectResourceAction->InitializeAction(this);
	CollectResourceData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/Peasant/T_IconCollectResource"));
	CollectResourceAction->SetActionData(CollectResourceData);
	
	AvailableActions.Add(BuildTownHallAction);
	AvailableActions.Add(CollectResourceAction);
}