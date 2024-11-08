// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/BaseAction.h"
#include "Entities/Actions/Interfaces/TargetTypeLocationAction.h"
#include "Entities/Actions/Interfaces/UnitAction.h"
#include "MoveToAction.generated.h"

struct FPathFollowingResult;
struct FAIRequestID;
class ARGUnitAIController;
class ARGUnitBase;

UCLASS()
class RTSGAME_API UMoveToAction : public UBaseAction, public IUnitAction, public ITargetTypeLocationAction
{
	GENERATED_BODY()

  public:
	virtual void Execute() override;
	
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void SetDestination(FVector InDestination) override;

  private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	ARGUnitAIController* AIController;
	FVector Destination;
};
