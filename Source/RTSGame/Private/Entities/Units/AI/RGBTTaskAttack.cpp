// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/AI/RGBTTaskAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/RGUnitBase.h"
#include "Entities/Units/RGUnitAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(RGBTTaskAttack, All, All);

void URGBTTaskAttack::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type URGBTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(RGBTTaskAttack, Warning, TEXT("[ExecuteTask]: !AIController"));
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(RGBTTaskAttack, Warning, TEXT("[ExecuteTask]: !ControlledPawn"));
		return EBTNodeResult::Failed;
	}

	ARGUnitBase* Unit = Cast<ARGUnitBase>(ControlledPawn);
	if (!Unit)
	{
		UE_LOG(RGBTTaskAttack, Warning, TEXT("[ExecuteTask]: !Unit"));
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Unit->GetMesh()->GetAnimInstance();
	URGUnitAnimInstance* UnitAnimInstance = Cast<URGUnitAnimInstance>(AnimInstance);
	if (!UnitAnimInstance)
	{
		UE_LOG(RGBTTaskAttack, Warning, TEXT("[ExecuteTask]: !UnitAnimInstance"));
		return EBTNodeResult::Failed;
	}

	UnitAnimInstance->SetIsAttacking(true);

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		UE_LOG(RGBTTaskAttack, Warning, TEXT("[ExecuteTask]: !BlackboardComponent"));
		return EBTNodeResult::Failed;
	}

	FVector TargetLocation = BlackboardComponent->GetValueAsVector("TargetLocationToMove");
	FVector CurrentLocation = Unit->GetActorLocation();
	FRotator CurrentRotation = Unit->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;

	Unit->SetActorRotation(TargetRotation);
	
	return EBTNodeResult::Succeeded;
}