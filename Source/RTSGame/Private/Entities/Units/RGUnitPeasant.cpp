// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"

#include "Entities/Actions/Implementation/CollectResourceAction.h"
#include "Entities/Actions/Implementation/ConstructBuildingAction.h"
#include "Entities/Buildings/RGBuildingTownHall.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"

ARGUnitPeasant::ARGUnitPeasant() : PlayerPawn(nullptr), CarryingWood(0)
{
	UnitImportance = EFEntitiesImportance::Peasant;
}

void ARGUnitPeasant::AddCarryingWood(int32 Amount)
{
	CarryingWood += Amount;
}

bool ARGUnitPeasant::GetIsCarryingResources() const
{
	return CarryingWood != 0;
}

void ARGUnitPeasant::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	InitializeActions();
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

	AvailableActions.Add(BuildTownHallAction);

	UCollectResourceAction* CollectResourceAction = NewObject<UCollectResourceAction>();
	FActionData CollectResourceData = UnitActions::Peasant_CollectResource;
	CollectResourceAction->InitializeAction(this);
	CollectResourceData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/Peasant/T_IconCollectResource"));
	CollectResourceAction->SetActionData(CollectResourceData);
	
	AvailableActions.Add(CollectResourceAction);
}