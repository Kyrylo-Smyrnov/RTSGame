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
	void AddSubAction(UBaseAction* Action);
	virtual void Execute_Implementation() override;
	virtual void Cancel_Implementation() override;
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	
  protected:
	void OnSubActionCompleted();
	
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	TArray<UBaseAction*> SubActions;
};
