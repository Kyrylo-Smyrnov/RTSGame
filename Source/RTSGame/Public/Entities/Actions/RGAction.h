// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGActionData.h"
#include "UObject/Interface.h"
#include "RGAction.generated.h"

UINTERFACE(MinimalAPI)
class URGAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IRGAction
{
	GENERATED_BODY()
  public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Execute();

	void SetActionData(const FRGActionData& InActionData);
	FRGActionData& GetActionData();

  protected:
	FRGActionData ActionData;
};
