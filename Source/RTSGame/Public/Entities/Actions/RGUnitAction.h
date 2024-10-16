// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGUnitAction.generated.h"

class ARGUnitBase;

UINTERFACE(MinimalAPI)
class URGUnitAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IRGUnitAction
{
	GENERATED_BODY()

  public:
	virtual void InitializeAction(ARGUnitBase* Unit) = 0;
};