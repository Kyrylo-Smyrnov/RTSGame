// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/CompositeAction.h"

void UCompositeAction::AddSubAction(UBaseAction* Action)
{
	SubActions.Add(Action);
}

void UCompositeAction::Execute()
{
	if (SubActions.Num() > 0)
	{
		UBaseAction* CurrentSubaction = SubActions[0];

		CurrentSubaction->Execute();
		CurrentSubaction->OnActionCompletedDelegate().AddUObject(this, &UCompositeAction::OnSubActionCompleted);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}

void UCompositeAction::Cancel()
{
	for (int32 i = 0; i < SubActions.Num(); ++i)
	{
		UBaseAction* Action = SubActions[i];
		if (Action)
		{
			Action->Cancel();
		}
	}

	SubActions.Empty();
}

void UCompositeAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
}

void UCompositeAction::OnSubActionCompleted()
{
	if (SubActions.Num() > 0)
	{
		SubActions.RemoveAt(0);
		Execute();
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}