// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGAction.h"

void IRGAction::SetActionData(const FRGActionData& InActionData)
{
	ActionData = InActionData;
}

FRGActionData& IRGAction::GetActionData()
{
	return ActionData;
}

FOnActionCompleted& IRGAction::OnActionCompletedDelegate()
{
	return OnActionCompleted;
}