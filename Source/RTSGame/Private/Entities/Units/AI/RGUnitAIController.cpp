// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGUnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Actions/RGMoveToAction.h"
#include "Entities/BBKeys.h"
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
	else if (!Unit->GetIsSelected())
		return;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		URGMoveToAction* MoveAction = Unit->GetMoveToAction();
		if (MoveAction)
		{
			MoveAction->SetDestination(HitResult.Location);
			MoveAction->Execute_Implementation();
		}
	}
}
