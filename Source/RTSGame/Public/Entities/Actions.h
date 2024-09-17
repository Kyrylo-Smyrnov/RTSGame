// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actionable.h"
#include "Player/RGPlayerPawn.h"

namespace ActionsUtility
{
bool IsEnoughResourcesToBuild(ARGPlayerPawn* PlayerPawn, int32 WoodCost);
}

namespace BuildingActions
{
extern FActionData TownHall_BuildPeasant;
} // namespace BuildingActions

namespace UnitActions
{
extern FActionData Base_Attack;
extern FActionData Base_Hold;
extern FActionData Base_Move;
extern FActionData Base_MoveAttack;

extern FActionData Peasant_BuildTownHall;
} // UnitActions