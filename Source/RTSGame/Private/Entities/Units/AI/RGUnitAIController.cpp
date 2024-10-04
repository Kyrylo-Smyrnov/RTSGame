// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGUnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	else if(!Unit->GetIsSelected())
		return;

	FHitResult HitResult;
	if(!PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
	{
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

		BlackboardComponent->SetValueAsEnum(BBKeys::UNIT_AI_BBKEY_UNITSTATE, 0);
		
		if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
			BlackboardComponent->SetValueAsVector(BBKeys::UNIT_AI_BBKEY_TARGETLOCATIONTOMOVE, HitResult.Location);
	}
}
