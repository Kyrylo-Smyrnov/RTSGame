// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/MoveToAction.h"
#include "AITypes.h"
#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Units/RGUnitBase.h"
#include "Navigation/PathFollowingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGMoveToAction, All, All)

UMoveToAction::UMoveToAction()
	: ControlledUnit(nullptr), AIController(nullptr), Destination(FVector::ZeroVector)
{
}

void UMoveToAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	AIController = Cast<ARGUnitAIController>(ControlledUnit->GetController());
}

void UMoveToAction::SetDestination(FVector InDestination)
{
	Destination = InDestination;
}

void UMoveToAction::Execute_Implementation()
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
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMoveToAction::OnMoveCompleted);
	}
	else
	{
		OnActionCompletedDelegate().Broadcast();
	}
}

void UMoveToAction::Cancel_Implementation()
{
}

void UMoveToAction::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{	
	if (AIController && AIController->GetPathFollowingComponent())
		AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	OnActionCompletedDelegate().Broadcast();
}