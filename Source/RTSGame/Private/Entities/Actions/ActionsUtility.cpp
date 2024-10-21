// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/ActionsUtility.h"

namespace ActionsUtility
{
bool IsEnoughResourcesToBuild(ARGPlayerPawn* PlayerPawn, const int32 WoodCost)
{
	const int32 PlayerResources = PlayerPawn->GetPlayerResources();
	
	return PlayerResources >= WoodCost;
}
}