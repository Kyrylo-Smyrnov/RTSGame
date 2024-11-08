// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/CompositeAction.h"
#include "Entities/Actions/Interfaces/TargetTypeActorAction.h"
#include "CollectResourceAction.generated.h"

UCLASS()
class RTSGAME_API UCollectResourceAction : public UCompositeAction, public ITargetTypeActorAction
{
	GENERATED_BODY()

  public:
	virtual void Execute() override;
	
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void SetTarget(AActor* InTarget) override;

  private:
	UPROPERTY()
	AActor* Target;
};
