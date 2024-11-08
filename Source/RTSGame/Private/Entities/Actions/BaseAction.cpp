// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/BaseAction.h"

void UBaseAction::Execute()
{
}

void UBaseAction::Cancel()
{
}

void UBaseAction::SetActionData(FActionData& InActionData)
{
	ActionData = InActionData;
}

FActionData& UBaseAction::GetActionData()
{
	return ActionData;
}

FOnActionCompleted& UBaseAction::OnActionCompletedDelegate()
{
	return OnActionCompleted;
}