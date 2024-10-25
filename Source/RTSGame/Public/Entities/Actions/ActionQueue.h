// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "ActionQueue.generated.h"

class UBaseAction;
class ARGUnitBase;

UCLASS()
class RTSGAME_API UActionQueue : public UObject
{
	GENERATED_BODY()

  public:
	void Initialize(ARGUnitBase* InUnit);
	void EnqueueAction(UBaseAction* NewAction);
	void ExecuteNextAction();
	void OnActionCompleted();
	void ClearQueue();

  private:
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	TArray<UBaseAction*> ActionQueue;
};
