// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGAction.h"
#include "RGMoveToAction.generated.h"

class ARGUnitAIController;
class ARGUnitBase;

UCLASS()
class RTSGAME_API URGMoveToAction : public UObject, public IRGAction
{
	GENERATED_BODY()

  public:
	URGMoveToAction();
	void InitializeAction(ARGUnitBase* InControlledUnit);
	virtual void Execute_Implementation() override;

	void SetDestination(FVector InDestination);

  private:
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	ARGUnitAIController* AIController;
	FVector Destination;
};
