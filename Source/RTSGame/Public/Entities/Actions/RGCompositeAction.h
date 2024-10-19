// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGAction.h"
#include "RGCompositeAction.generated.h"

UCLASS()
class RTSGAME_API URGCompositeAction : public UObject, public IRGAction
{
	GENERATED_BODY()

  public:
	void AddSubAction(IRGAction* Action);
	virtual void Execute_Implementation() override;
  protected:
	void OnSubActionCompleted();
	
	TArray<IRGAction*> SubActions;
};
