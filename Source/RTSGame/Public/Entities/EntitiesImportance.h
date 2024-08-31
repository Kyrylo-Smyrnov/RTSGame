// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

// Importance of entities, which determines the HUD of which entity to show,
// when adding and removing an array of selected entities is sorted and the HUD
// of the first (most important) one is shown.
enum EFEntitiesImportance
{
	// Buildings
	TownHall = 0,

	// Units
	Peasant = 1
};