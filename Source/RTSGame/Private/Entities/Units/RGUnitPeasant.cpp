// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitPeasant.h"
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