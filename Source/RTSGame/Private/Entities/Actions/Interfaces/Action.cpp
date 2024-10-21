// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Interfaces/Action.h"

void IAction::SetActionData(const FActionData& InActionData)
{
	ActionData = InActionData;
}

FActionData& IAction::GetActionData()
{
	return ActionData;
}

FOnActionCompleted& IAction::OnActionCompletedDelegate()
{
	return OnActionCompleted;
}