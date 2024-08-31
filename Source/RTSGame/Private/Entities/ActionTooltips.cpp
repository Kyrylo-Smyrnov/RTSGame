#include "Entities/ActionsTooltips.h"

namespace Tooltips
{
namespace Units
{
FText AttackActionTooltip = FText::FromString("Order the selected unit to attack the targeted enemy.");
FText HoldActionTooltip = FText::FromString("Command the selected unit to hold its current position.");
FText MoveActionTooltip = FText::FromString("Command the selected unit to move to the targeted location.");
FText MoveAttackActionTooltip = FText::FromString(
	"Command the selected unit to move towards a target while attacking any enemies encountered along the way.");
namespace Peasant
{
FText BuildTownHall = FText::FromString(
	"Establish a Town Hall to enhance your base's capabilities. The Town Hall enables you to recruit peasants.");
} // namespace Peasant
} // namespace Units

namespace Buildings
{
namespace TownHall
{
FText BuildPeasantTooltip = FText::FromString(
	"Capable of constructing various buildings to expand your base and strengthen your position.");
} // namespace TownHall
} // namespace Buildings
} // namespace Tooltips