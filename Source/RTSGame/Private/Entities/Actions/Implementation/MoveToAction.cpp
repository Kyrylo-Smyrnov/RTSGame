// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/MoveToAction.h"
#include "AITypes.h"
#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Units/RGUnitBase.h"
#include "Navigation/PathFollowingComponent.h"

void UMoveToAction::Execute()
{
	if (!ControlledUnit || !AIController || Destination == FVector::ZeroVector)
	{
		return;
	}

	FNavPathSharedPtr NavPath;
	FAIMoveRequest MoveRequest(Destination);
	FPathFollowingRequestResult RequestResult = AIController->MoveTo(MoveRequest, &NavPath);

	if (RequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMoveToAction::OnMoveCompleted);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}

void UMoveToAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	AIController = Cast<ARGUnitAIController>(ControlledUnit->GetController());
}

void UMoveToAction::SetDestination(const FVector InDestination)
{
	Destination = InDestination;
}

void UMoveToAction::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (AIController && AIController->GetPathFollowingComponent())
		AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	OnActionCompletedDelegate().Broadcast();
}