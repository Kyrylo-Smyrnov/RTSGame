// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "BaseAction.h"
#include "RGAction.h"
#include "RGTargetTypeActorAction.h"
#include "RGUnitAction.h"
#include "RGAttackAction.generated.h"

UCLASS()
class RTSGAME_API URGAttackAction : public UBaseAction, public IRGUnitAction, public IRGTargetTypeActorAction
{
	GENERATED_BODY()
	
  public:
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void Execute_Implementation() override;
	virtual void SetTarget(AActor* InTarget) override;

  private:
	void ExecuteAttack();
	void StopAttack();
	
	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	AActor* Target;

	FTimerHandle AttackTimerHandle;

	int32 DamageAmount = 10;
	int32 AttackSpeed = 1;
	int32 AttackRange = 200;
};
