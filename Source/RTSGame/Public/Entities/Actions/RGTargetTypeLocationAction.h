// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGTargetTypeLocationAction.generated.h"

UINTERFACE(MinimalAPI)
class URGTargetTypeLocationAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IRGTargetTypeLocationAction
{
	GENERATED_BODY()

  public:
	virtual void SetDestination(FVector InDestination) = 0;
};
