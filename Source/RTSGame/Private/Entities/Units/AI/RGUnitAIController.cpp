// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGUnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "RGPlayerController.h"

void ARGUnitAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->RightMouseButtonInputPressedUninteractable.AddUObject(
		this, &ARGUnitAIController::HandleRightMouseButtonInputPressedUninteractable);
}

void ARGUnitAIController::HandleRightMouseButtonInputPressedUninteractable()
{
	ARGUnitBase* Unit = Cast<ARGUnitBase>(GetPawn());
	if (Unit->IsSelected())
	{
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
		
		BlackboardComponent->SetValueAsEnum("UnitState", 0);
		
		FHitResult HitResult;
		if(PlayerController->GetHitResultUnderCursorByChannel(
			UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
		{
			BlackboardComponent->SetValueAsVector("TargetLocationToMove", HitResult.Location);
		}
	}
}
