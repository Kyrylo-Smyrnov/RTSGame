// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/ActionData.h"
#include "UObject/Interface.h"
#include "Action.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionCompleted);

UINTERFACE(MinimalAPI)
class UAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IAction
{
	GENERATED_BODY()

  public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Execute();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Cancel();
	
	void SetActionData(const FActionData& InActionData);
	FActionData& GetActionData();

	virtual FOnActionCompleted& OnActionCompletedDelegate();

  protected:
	FActionData ActionData;
	FOnActionCompleted OnActionCompleted;
};
