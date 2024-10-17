// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGAction.h"
#include "RGTargetTypeLocationAction.h"
#include "RGUnitAction.h"
#include "RGMoveToAction.generated.h"

struct FPathFollowingResult;
struct FAIRequestID;
class ARGUnitAIController;
class ARGUnitBase;

UCLASS()
class RTSGAME_API URGMoveToAction : public UObject, public IRGAction, public IRGUnitAction, public IRGTargetTypeLocationAction
{
	GENERATED_BODY()

  public:
	URGMoveToAction();
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void SetDestination(FVector InDestination) override;
	
	virtual void Execute_Implementation() override;

  private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	ARGUnitAIController* AIController;
	FVector Destination;
};
