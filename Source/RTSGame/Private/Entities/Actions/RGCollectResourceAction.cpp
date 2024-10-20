// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGCollectResourceAction.h"
#include "Entities/Actions/RGAttackAction.h"
#include "Entities/Actions/RGMoveToAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGCollectResourceAction, All, All);

void URGCollectResourceAction::InitializeAction(ARGUnitBase* Unit)
{
	Super::InitializeAction(Unit);
}

void URGCollectResourceAction::SetTarget(AActor* InTarget)
{
	Target = InTarget;

	URGMoveToAction* MoveToAction = NewObject<URGMoveToAction>(this);
	MoveToAction->InitializeAction(ControlledUnit);
	MoveToAction->SetDestination(Target->GetActorLocation());
	AddSubAction(MoveToAction);

	URGAttackAction* AttackAction = NewObject<URGAttackAction>(this);
	AttackAction->InitializeAction(ControlledUnit);
	AttackAction->SetTarget(Target);
	AddSubAction(AttackAction);
}

void URGCollectResourceAction::Execute_Implementation()
{
	if (!ControlledUnit)
	{
		UE_LOG(LogRGCollectResourceAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	Super::Execute_Implementation();
}
