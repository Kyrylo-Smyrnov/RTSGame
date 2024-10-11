// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Player/RGPlayerPawn.h"

namespace ActionsUtility
{
bool IsEnoughResourcesToBuild(ARGPlayerPawn* PlayerPawn, const int32 WoodCost);
}