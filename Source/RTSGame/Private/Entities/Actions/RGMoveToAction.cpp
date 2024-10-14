// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGMoveToAction.h"
#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Units/RGUnitBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGMoveToAction, All, All)

URGMoveToAction::URGMoveToAction()
	: ControlledUnit(nullptr), AIController(nullptr), Destination(FVector::ZeroVector)
{
}

void URGMoveToAction::InitializeAction(ARGUnitBase* InControlledUnit)
{
	ControlledUnit = InControlledUnit;
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

	AIController->MoveToLocation(Destination, 50.0f, true);
}

void URGMoveToAction::SetDestination(FVector InDestination)
{
	Destination = InDestination;
}