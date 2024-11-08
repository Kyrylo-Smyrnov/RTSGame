// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/AttackAction.h"
#include "Entities/AttackableBase.h"
#include "Entities/Units/Animation/RGUnitAnimInstance.h"
#include "Entities/Units/RGUnitBase.h"

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

void UAttackAction::Execute()
{
	if (!ControlledUnit || !Target)
	{
		return;
	}

	FVector DirectionToTarget = (Target->GetActorLocation() - ControlledUnit->GetActorLocation()).GetSafeNormal();
	FRotator RotationToTarget = FRotator(ControlledUnit->GetActorRotation().Pitch, DirectionToTarget.Rotation().Yaw, DirectionToTarget.Rotation().Roll);
	ControlledUnit->SetActorRotation(RotationToTarget);

	if (UnitAnimInstance)
	{
		UnitAnimInstance->SetIsAttacking(true);
	}
	
	ControlledUnit->GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UAttackAction::ExecuteAttack, AttackSpeed, true);
}

void UAttackAction::Cancel()
{
	StopAttack(nullptr);
}

void UAttackAction::ExecuteAttack()
{
	if (!Target || Target->IsPendingKill() || !ControlledUnit || ControlledUnit->IsPendingKill())
	{
		StopAttack(nullptr);
		return;
	}

	float DistanceToTarget = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
	if (DistanceToTarget > AttackRange)
	{
		StopAttack(nullptr);
		return;
	}

	if(AAttackableBase* Attackable = Cast<AAttackableBase>(Target))
	{
		Attackable->ReceiveDamage(DamageAmount, ControlledUnit);
	}
}

void UAttackAction::StopAttack(AActor* DestroyedActor)
{
	if (AttackTimerHandle.IsValid() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
	
	if (Target)
	{
		Target->OnDestroyed.RemoveDynamic(this, &UAttackAction::StopAttack);
	}

	if (UnitAnimInstance)
	{
		UnitAnimInstance->SetIsAttacking(false);
	}

	OnActionCompletedDelegate().Broadcast();
}