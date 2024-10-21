// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetTypeLocationAction.generated.h"

UINTERFACE(MinimalAPI)
class UTargetTypeLocationAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API ITargetTypeLocationAction
{
	GENERATED_BODY()

  public:
	virtual void SetDestination(FVector InDestination) = 0;
};
