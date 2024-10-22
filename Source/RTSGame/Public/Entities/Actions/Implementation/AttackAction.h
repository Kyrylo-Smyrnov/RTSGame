// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/BaseAction.h"
#include "Entities/Actions/Interfaces/TargetTypeActorAction.h"
#include "Entities/Actions/Interfaces/UnitAction.h"
#include "AttackAction.generated.h"

class URGUnitAnimInstance;
class ARGPlayerPawn;

UCLASS()
class RTSGAME_API UAttackAction : public UBaseAction, public IUnitAction, public ITargetTypeActorAction
{
	GENERATED_BODY()

  public:
	virtual void InitializeAction(ARGUnitBase* Unit) override;
	virtual void Execute_Implementation() override;
	virtual void Cancel_Implementation() override;
	virtual void SetTarget(AActor* InTarget) override;

  private:
	void ExecuteAttack();
	UFUNCTION()
	void StopAttack(AActor* DestroyedActor);

	UPROPERTY()
	ARGUnitBase* ControlledUnit;
	UPROPERTY()
	AActor* Target;
	UPROPERTY()
	URGUnitAnimInstance* UnitAnimInstance;

	FTimerHandle AttackTimerHandle;

	int32 DamageAmount = 10;
	int32 AttackSpeed = 1;
	int32 AttackRange = 300;
};
