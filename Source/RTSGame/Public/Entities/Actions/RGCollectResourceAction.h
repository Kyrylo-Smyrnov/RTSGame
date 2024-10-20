// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGTargetTypeActorAction.h"
#include "Entities/Actions/RGCompositeAction.h"
#include "RGUnitAction.h"
#include "RGCollectResourceAction.generated.h"

class ARGResourceBase;

UCLASS()
class RTSGAME_API URGCollectResourceAction : public URGCompositeAction, public IRGTargetTypeActorAction
{
	GENERATED_BODY()

  public:
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void Execute_Implementation() override;
	virtual void SetTarget(AActor* InTarget) override;

  private:
	UPROPERTY()
	AActor* Target;
};
