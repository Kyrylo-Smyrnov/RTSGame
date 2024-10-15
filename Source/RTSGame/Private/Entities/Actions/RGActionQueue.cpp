// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGActionQueue.h"
#include "Entities/Actions/RGAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGActionQueue, All, All);

void URGActionQueue::Initialize(ARGUnitBase* InUnit)
{
	ControlledUnit = InUnit;
}

void URGActionQueue::EnqueueAction(IRGAction* NewAction)
{
	ActionQueue.Add(NewAction);
	if (ActionQueue.Num() == 1)
		ExecuteNextAction();
}

void URGActionQueue::ExecuteNextAction()
{
	if(ActionQueue.Num() > 0)
	{
		IRGAction* CurrentAction = ActionQueue[0];
		CurrentAction->Execute_Implementation();
		CurrentAction->OnActionCompletedDelegate().AddUObject(this, &URGActionQueue::OnActionCompleted);
	}
}

void URGActionQueue::OnActionCompleted()
{
	if (ActionQueue.Num() > 0)
		ActionQueue.RemoveAt(0);

	ExecuteNextAction();
}

void URGActionQueue::ClearQueue()
{
	ActionQueue.Empty();
}
