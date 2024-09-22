// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions.h"

namespace ActionsUtility
{
bool IsEnoughResourcesToBuild(ARGPlayerPawn* PlayerPawn, int32 WoodCost)
{
	int32 PlayerResources = PlayerPawn->GetPlayerResources();
	
	return PlayerResources >= WoodCost;
}
}

namespace BuildingActions
{
FActionData TownHall_BuildPeasant = FActionData{
	"BuildPeasant",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Buildings/TownHall/T_IconBuildPeasant")),
	FText::FromString("Capable of constructing various buildings to expand your base and strengthen your position."),
	0,
	0,
	0,
	0,
	5};
} // namespace BuildingActions

namespace UnitActions
{
// Base
FActionData Base_Attack = FActionData{
	"AttackAction",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconAttack")),
	FText::FromString("Order the selected unit to attack the targeted enemy."),
	0,
	0,
	0,
	0,
	0};

FActionData Base_Hold = FActionData{
	"HoldAction",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconHold")),
	FText::FromString("Command the selected unit to hold its current position."),
	0,
	0,
	0,
	0,
	0};

FActionData Base_Move = FActionData{
	"MoveAction",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMove")),
	FText::FromString("Command the selected unit to move to the targeted location."),
	0,
	0,
	0,
	0,
	0};

FActionData Base_MoveAttack = FActionData{
	"MoveAttackAction",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMoveAttack")),
	FText::FromString("Command the selected unit to move towards a target while attacking any enemies encountered along the way."),
	0,
	0,
	0,
	0,
	0};

// Peasant
FActionData Peasant_BuildTownHall = FActionData{
	"BuildTownHall",
	LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/Peasant/T_IconBuildTownHall")),
	FText::FromString("Capable of constructing various buildings to expand your base and strengthen your position."),
	0,
	0,
	0,
	0,
	2};

} // namespace UnitActions