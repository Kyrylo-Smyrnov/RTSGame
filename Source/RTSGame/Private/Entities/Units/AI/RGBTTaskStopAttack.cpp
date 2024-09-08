// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGBTTaskStopAttack.h"
#include "AIController.h"
#include "Entities/RGUnitBase.h"
#include "Entities/Units/RGUnitAnimInstance.h"

DEFINE_LOG_CATEGORY_STATIC(RGBTTaskStopAttack, All, All);

void URGBTTaskStopAttack::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type URGBTTaskStopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(RGBTTaskStopAttack, Warning, TEXT("[ExecuteTask]: !AIController"));
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if(!ControlledPawn)
	{
		UE_LOG(RGBTTaskStopAttack, Warning, TEXT("[ExecuteTask]: !ControlledPawn"));
		return EBTNodeResult::Failed;
	}

	ARGUnitBase* Unit = Cast<ARGUnitBase>(ControlledPawn);
	if(!Unit)
	{
		UE_LOG(RGBTTaskStopAttack, Warning, TEXT("[ExecuteTask]: !Unit"));
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Unit->GetMesh()->GetAnimInstance();
	URGUnitAnimInstance* UnitAnimInstance = Cast<URGUnitAnimInstance>(AnimInstance);
	if(!UnitAnimInstance)
	{
		UE_LOG(RGBTTaskStopAttack, Warning, TEXT("[ExecuteTask]: !UnitAnimInstance"));
		return EBTNodeResult::Failed;
	}

	UnitAnimInstance->SetIsAttacking(false);
	return EBTNodeResult::Succeeded;
}