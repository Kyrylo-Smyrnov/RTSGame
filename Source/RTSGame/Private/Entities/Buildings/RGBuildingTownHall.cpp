// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"

#include "Entities/Actions/ActionsList.h"
#include "Entities/Units/RGUnitPeasant.h"

DEFINE_LOG_CATEGORY_STATIC(LogBuildingTownHall, All, All);

ARGBuildingTownHall::ARGBuildingTownHall()
	: ARGBuildingBase()
{
	this->BuildingImportance = EFEntitiesImportance::TownHall;
}

void ARGBuildingTownHall::BeginPlay()
{
	Super::BeginPlay();
}

void ARGBuildingTownHall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARGBuildingTownHall::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Super::HandleOnClicked(TouchedActor, ButtonPressed);
}

void ARGBuildingTownHall::InitializeActions()
{
	Super::InitializeActions();
	
	USpawnUnitAction* SpawnPeasantAction = NewObject<USpawnUnitAction>();
	SpawnPeasantAction->InitializeAction(UnitPeasantBlueprintClass, this, PlayerPawn);
	FActionData BuildPeasantData = BuildingActions::TownHall_BuildPeasant;
	BuildPeasantData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Buildings/TownHall/T_IconBuildPeasant"));
	SpawnPeasantAction->SetActionData(BuildPeasantData);
	
	AvailableActions.Add(SpawnPeasantAction);
}
