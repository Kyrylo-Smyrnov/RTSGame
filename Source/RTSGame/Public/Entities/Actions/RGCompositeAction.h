// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "BaseAction.h"
#include "RGAction.h"
#include "RGUnitAction.h"
#include "RGCompositeAction.generated.h"

UCLASS()
class RTSGAME_API URGCompositeAction : public UBaseAction, public IRGUnitAction
{
	GENERATED_BODY()

  public:
	void AddSubAction(IRGAction* Action);
	virtual void Execute_Implementation() override;
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	
  protected:
	void OnSubActionCompleted();
	
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	
	TArray<IRGAction*> SubActions;
};
