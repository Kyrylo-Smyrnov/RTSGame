// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/CollectResourceAction.h"
#include "Entities/Actions/Implementation/AttackAction.h"
#include "Entities/Actions/Implementation/MoveToAction.h"

void UCollectResourceAction::InitializeAction(ARGUnitBase* Unit)
{
	Super::InitializeAction(Unit);
}

void UCollectResourceAction::SetTarget(AActor* InTarget)
{
	Target = InTarget;

	UMoveToAction* MoveToAction = NewObject<UMoveToAction>(this);
	MoveToAction->InitializeAction(ControlledUnit);
	MoveToAction->SetDestination(Target->GetActorLocation());
	AddSubAction(MoveToAction);

	UAttackAction* AttackAction = NewObject<UAttackAction>(this);
	AttackAction->InitializeAction(ControlledUnit);
	AttackAction->SetTarget(Target);
	AddSubAction(AttackAction);
}

void UCollectResourceAction::Execute()
{
	if (!ControlledUnit)
	{
		return;
	}

	Super::Execute();
}