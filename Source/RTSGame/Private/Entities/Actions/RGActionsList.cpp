// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGActionsList.h"

namespace BuildingActions
{
FRGActionData TownHall_BuildPeasant = FRGActionData{
	"BuildPeasant",
	nullptr,
	FText::FromString("Capable of constructing various buildings to expand your base and strengthen your position."),
	0,
	0,
	0,
	0,
	5,
	EActionTargetType::None};
} // namespace BuildingActions

namespace UnitActions
{
FRGActionData Base_Move = FRGActionData{
	"MoveTo",
	nullptr,
	FText::FromString(""),
	0,
	0,
	0,
	0,
	0,
	EActionTargetType::Location};
// Peasant
FRGActionData Peasant_BuildTownHall = FRGActionData{
	"BuildTownHall",
	nullptr,
	FText::FromString("Capable of constructing various buildings to expand your base and strengthen your position."),
	0,
	0,
	0,
	0,
	10,
	EActionTargetType::None
};

FRGActionData Peasant_CollectResource = FRGActionData{
	"CollectResource",
	nullptr,
	FText::FromString(""),
	0,
	0,
	0,
	0,
	0,
	EActionTargetType::Actor
};
} // namespace UnitActions