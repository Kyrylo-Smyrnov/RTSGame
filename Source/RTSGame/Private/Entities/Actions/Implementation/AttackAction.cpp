// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/AttackAction.h"
#include "Entities/RGAttackable.h"
#include "Entities/Units/Animation/RGUnitAnimInstance.h"
#include "Entities/Units/RGUnitBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogAttackAction, All, All)

void UAttackAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	UnitAnimInstance = Cast<URGUnitAnimInstance>(ControlledUnit->GetMesh()->GetAnimInstance());
}

void UAttackAction::SetTarget(AActor* InTarget)
{
	Target = InTarget;
	Target->OnDestroyed.AddDynamic(this, &UAttackAction::StopAttack);
}

void UAttackAction::Execute_Implementation()
{
	if (!ControlledUnit)
	{
		UE_LOG(LogAttackAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if (!Target)
	{
		UE_LOG(LogAttackAction, Warning, TEXT("[Execute] It is necessary to set up the target before execution."));
		return;
	}

	FVector DirectionToTarget = (Target->GetActorLocation() - ControlledUnit->GetActorLocation()).GetSafeNormal();
	FRotator RotationToTarget = FRotator(ControlledUnit->GetActorRotation().Pitch, DirectionToTarget.Rotation().Yaw, DirectionToTarget.Rotation().Roll);
	ControlledUnit->SetActorRotation(RotationToTarget);

	if (UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(true);
	
	ControlledUnit->GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UAttackAction::ExecuteAttack, AttackSpeed, true);
}

void UAttackAction::Cancel_Implementation()
{
	if (AttackTimerHandle.IsValid() && ControlledUnit->GetWorld())
		ControlledUnit->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	
	if (Target)
		Target->OnDestroyed.RemoveDynamic(this, &UAttackAction::StopAttack);

	if (UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(false);
}

void UAttackAction::ExecuteAttack()
{
	if (!Target || Target->IsPendingKill() || !ControlledUnit || ControlledUnit->IsPendingKill())
	{
		StopAttack(Target);
		return;
	}

	float DistanceToTarget = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
	if (DistanceToTarget > AttackRange)
	{
		StopAttack(Target);
		return;
	}

	if (Target->GetClass()->ImplementsInterface(URGAttackable::StaticClass()))
	{
		IRGAttackable* AttackableActor = Cast<IRGAttackable>(Target);
		AttackableActor->ReceiveDamage(DamageAmount, ControlledUnit);
	}
}

void UAttackAction::StopAttack(AActor* DestroyedActor)
{
	if (AttackTimerHandle.IsValid() && ControlledUnit->GetWorld())
			ControlledUnit->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	
	if (Target)
		Target->OnDestroyed.RemoveDynamic(this, &UAttackAction::StopAttack);

	if (UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(false);

	OnActionCompletedDelegate().Broadcast();
}