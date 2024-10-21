// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitAction.generated.h"

class ARGUnitBase;

UINTERFACE(MinimalAPI)
class UUnitAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IUnitAction
{
	GENERATED_BODY()

  public:
	virtual void InitializeAction(ARGUnitBase* Unit) = 0;
};