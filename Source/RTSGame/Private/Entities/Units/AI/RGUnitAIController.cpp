// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Actions/RGMoveToAction.h"
#include "Entities/Units/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGUnitAIController, All, All);

void ARGUnitAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
		PlayerController->RightMouseButtonInputPressed.AddUObject(this, &ARGUnitAIController::HandleRightMouseButtonInputPressed);
	else
		UE_LOG(LogRGUnitAIController, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));
}

void ARGUnitAIController::HandleRightMouseButtonInputPressed()
{
	ARGUnitBase* Unit = Cast<ARGUnitBase>(GetPawn());
	if (!Unit)
	{
		UE_LOG(LogRGUnitAIController, Warning, TEXT("[HandleRightMouseButtonInputPressed] Unit is nullptr."));
		return;
	}
	
	if (!Unit->GetIsSelected())
		return;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		URGMoveToAction* MoveAction = NewObject<URGMoveToAction>(Unit);
		FRGActionData MoveToData = UnitActions::Base_Move;
		MoveAction->InitializeAction(Unit);
		MoveAction->SetActionData(MoveToData);
		MoveAction->SetDestination(HitResult.Location);

		if (PlayerController->IsInputKeyDown(EKeys::LeftShift))
		{
			Unit->AddActionToQueue(MoveAction);
		}
		else
		{
			Unit->ClearActionQueue();
			Unit->AddActionToQueue(MoveAction);
		}
	}
	else if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult))
	{
		URGMoveToAction* MoveAction = NewObject<URGMoveToAction>(Unit);
		FRGActionData MoveToData = UnitActions::Base_Move;
		MoveAction->InitializeAction(Unit);
		MoveAction->SetActionData(MoveToData);
		MoveAction->SetDestination(HitResult.GetActor()->GetActorLocation());

		if (PlayerController->IsInputKeyDown(EKeys::LeftShift))
		{
			Unit->AddActionToQueue(MoveAction);
		}
		else
		{
			Unit->ClearActionQueue();
			Unit->AddActionToQueue(MoveAction);
		}
	}
}
