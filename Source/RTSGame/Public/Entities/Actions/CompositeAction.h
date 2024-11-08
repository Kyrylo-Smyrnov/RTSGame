// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "BaseAction.h"
#include "Entities/Actions/Interfaces/UnitAction.h"
#include "CompositeAction.generated.h"

UCLASS()
class RTSGAME_API UCompositeAction : public UBaseAction, public IUnitAction
{
	GENERATED_BODY()

  public:
	virtual void Execute() override;
	virtual void Cancel() override;
	
	void AddSubAction(UBaseAction* Action);
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	
  protected:
	void OnSubActionCompleted();
	
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	TArray<UBaseAction*> SubActions;
};
