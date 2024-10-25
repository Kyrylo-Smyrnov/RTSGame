// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/Interfaces/Action.h"
#include "BaseAction.generated.h"

UCLASS()
class RTSGAME_API UBaseAction : public UObject, public IAction
{
	GENERATED_BODY()

  public:
	UBaseAction() = default;
};
