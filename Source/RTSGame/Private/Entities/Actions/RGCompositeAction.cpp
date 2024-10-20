// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGCompositeAction.h"

void URGCompositeAction::AddSubAction(IRGAction* Action)
{
	SubActions.Add(Action);
}

void URGCompositeAction::Execute_Implementation()
{
	if (SubActions.Num() > 0)
	{
		IRGAction* CurrentSubaction = SubActions[0];

		CurrentSubaction->Execute_Implementation();
		CurrentSubaction->OnActionCompletedDelegate().AddUObject(this, &URGCompositeAction::OnSubActionCompleted);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}

void URGCompositeAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
}

void URGCompositeAction::OnSubActionCompleted()
{
	if (SubActions.Num() > 0)
	{
		SubActions.RemoveAt(0);
		Execute_Implementation();
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}