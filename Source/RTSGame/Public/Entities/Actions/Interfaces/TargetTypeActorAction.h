// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetTypeActorAction.generated.h"

UINTERFACE(MinimalAPI)
class UTargetTypeActorAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API ITargetTypeActorAction
{
	GENERATED_BODY()

public:
	virtual void SetTarget(AActor* InTarget) = 0;
};

