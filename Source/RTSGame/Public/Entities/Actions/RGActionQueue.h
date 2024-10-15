// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGActionQueue.generated.h"

class ARGUnitBase;
class IRGAction;

UCLASS()
class RTSGAME_API URGActionQueue : public UObject
{
	GENERATED_BODY()

  public:
	void Initialize(ARGUnitBase* InUnit);
	void EnqueueAction(IRGAction* NewAction);
	void ExecuteNextAction();
	void OnActionCompleted();
	void ClearQueue();

  private:
	UPROPERTY()
	class ARGUnitBase* ControlledUnit;
	TArray<IRGAction*> ActionQueue;
};
