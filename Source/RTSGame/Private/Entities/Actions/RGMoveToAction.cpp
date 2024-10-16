// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGMoveToAction.h"
#include "AITypes.h"
#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Units/RGUnitBase.h"
#include "Navigation/PathFollowingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGMoveToAction, All, All)

URGMoveToAction::URGMoveToAction()
	: ControlledUnit(nullptr), AIController(nullptr), Destination(FVector::ZeroVector)
{
}

void URGMoveToAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	AIController = Cast<ARGUnitAIController>(ControlledUnit->GetController());
}

void URGMoveToAction::Execute_Implementation()
{
	if (!ControlledUnit || !AIController)
	{
		UE_LOG(LogRGMoveToAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if (Destination == FVector::ZeroVector)
	{
		UE_LOG(LogRGMoveToAction, Warning, TEXT("[Execute] It is necessary to set up the destination before execution."));
		return;
	}

	FNavPathSharedPtr NavPath;
	FAIMoveRequest MoveRequest(Destination);
	FPathFollowingRequestResult RequestResult = AIController->MoveTo(MoveRequest, &NavPath);

	if (RequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &URGMoveToAction::OnMoveCompleted);
	else
		UE_LOG(LogRGMoveToAction, Warning, TEXT("[Execute] MoveTo request failed."));
}

void URGMoveToAction::SetDestination(FVector InDestination)
{
	Destination = InDestination;
}

void URGMoveToAction::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	
	if (Result.Code == EPathFollowingResult::Success)
		OnActionCompletedDelegate().Broadcast();
}