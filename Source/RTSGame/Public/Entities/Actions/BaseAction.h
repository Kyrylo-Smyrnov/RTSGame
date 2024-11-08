// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "ActionData.h"
#include "CoreMinimal.h"
#include "BaseAction.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionCompleted);

UCLASS()
class RTSGAME_API UBaseAction : public UObject
{
	GENERATED_BODY()

  public:
	UBaseAction() = default;

	virtual void Execute();
	virtual void Cancel();

	void SetActionData(FActionData& InActionData);
	FActionData& GetActionData();

	FOnActionCompleted& OnActionCompletedDelegate();

  protected:
	FActionData ActionData;
	FOnActionCompleted OnActionCompleted;
};
