// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGUnitAIController.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
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
	if (!PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult) &&
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		UMoveToAction* MoveAction = NewObject<UMoveToAction>(Unit);
		MoveAction->InitializeAction(Unit);
		MoveAction->SetDestination(HitResult.Location);
	
		if (! PlayerController->IsInputKeyDown(EKeys::LeftShift))
		{
			Unit->ClearActionQueue();
		}
		
		Unit->AddActionToQueue(MoveAction);
	}
}
