// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/ActionsList.h"

namespace BuildingActions
{
FActionData TownHall_BuildPeasant = FActionData{
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
FActionData Base_Move = FActionData{
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
FActionData Peasant_BuildTownHall = FActionData{
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

FActionData Peasant_CollectResource = FActionData{
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