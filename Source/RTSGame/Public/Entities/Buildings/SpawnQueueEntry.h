// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "SpawnQueueEntry.generated.h"

USTRUCT()
struct FSpawnQueueEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> UnitClass;
	float SpawnTime;
};