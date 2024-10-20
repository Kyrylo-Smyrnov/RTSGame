// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGAction.h"
#include "BaseAction.generated.h"

UCLASS()
class RTSGAME_API UBaseAction : public UObject, public IRGAction
{
	GENERATED_BODY()
	
  public:
	UBaseAction() = default;
};
