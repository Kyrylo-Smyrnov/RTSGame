// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/CompositeAction.h"

void UCompositeAction::AddSubAction(UBaseAction* Action)
{
	SubActions.Add(Action);
}

void UCompositeAction::Execute_Implementation()
{
	if (SubActions.Num() > 0)
	{
		UBaseAction* CurrentSubaction = SubActions[0];

		CurrentSubaction->Execute_Execute(CurrentSubaction);
		CurrentSubaction->OnActionCompletedDelegate().AddUObject(this, &UCompositeAction::OnSubActionCompleted);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}

void UCompositeAction::Cancel_Implementation()
{
	for (int32 i = 0; i < SubActions.Num(); ++i)
	{
		UBaseAction* Action = SubActions[i];
		if (Action)
		{
			Action->Execute_Cancel(Action);
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
		Execute_Execute(this);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}