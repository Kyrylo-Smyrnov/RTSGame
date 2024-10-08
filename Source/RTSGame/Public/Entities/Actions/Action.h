// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGActionData.h"
#include "UObject/Interface.h"
#include "Action.generated.h"

UINTERFACE(MinimalAPI)
class UAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IAction
{
	GENERATED_BODY()
  public:
	virtual void Execute() = 0;
	virtual void SetActionData(const FRGActionData& InActionData) = 0;
};
