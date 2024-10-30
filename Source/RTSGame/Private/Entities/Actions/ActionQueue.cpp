// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/ActionQueue.h"
#include "Entities/Actions/BaseAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGActionQueue, All, All);

void UActionQueue::Initialize(ARGUnitBase* InUnit)
{
	ControlledUnit = InUnit;
}

void UActionQueue::EnqueueAction(UBaseAction* NewAction)
{
	ActionQueue.Add(NewAction);
	if (ActionQueue.Num() == 1)
		ExecuteNextAction();
}

void UActionQueue::ExecuteNextAction()
{
	if(ActionQueue.Num() > 0)
	{
		UBaseAction* CurrentAction = ActionQueue[0];
		CurrentAction->OnActionCompletedDelegate().AddUObject(this, &UActionQueue::OnActionCompleted);
		CurrentAction->Execute_Execute(CurrentAction);
	}
}

void UActionQueue::OnActionCompleted()
{
	if (ActionQueue.Num() > 0)
	{
		ActionQueue[0]->Execute_Cancel(ActionQueue[0]);
		ActionQueue.RemoveAt(0);
	}
	
	ExecuteNextAction();
}

void UActionQueue::ClearQueue()
{
	if(ActionQueue.Num() > 0)
	{
		if(ActionQueue[0])
			ActionQueue[0]->Execute_Cancel(ActionQueue[0]);
		
		ActionQueue.Empty();
	}
}
