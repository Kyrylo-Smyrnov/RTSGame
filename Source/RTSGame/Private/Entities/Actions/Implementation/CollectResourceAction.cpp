// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/CollectResourceAction.h"
#include "Entities/Actions/Implementation/AttackAction.h"
#include "Entities/Actions/Implementation/MoveToAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGCollectResourceAction, All, All);

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

void UCollectResourceAction::Execute_Implementation()
{
	if (!ControlledUnit)
	{
		UE_LOG(LogRGCollectResourceAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	Super::Execute_Implementation();
}

void UCollectResourceAction::Cancel_Implementation()
{
	Super::Cancel_Implementation();
}