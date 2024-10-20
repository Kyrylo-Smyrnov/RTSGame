// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/RGAttackAction.h"

#include "Entities/RGAttackable.h"
#include "Entities/Units/RGUnitBase.h"
#include "Entities/Units/Animation/RGUnitAnimInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGAttackAction, All, All)

void URGAttackAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	// DamageAmount = Unit->GetStatDamage();
	// AttackSpeed = Unit->GetStatAttackSpeed();
	// AttackRange = Unit->GetStatAttackRange();
}

void URGAttackAction::SetTarget(AActor* InTarget)
{
	Target = InTarget;
}

void URGAttackAction::Execute_Implementation()
{
	if (!ControlledUnit)
	{
		UE_LOG(LogRGAttackAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if (!Target)
	{
		UE_LOG(LogRGAttackAction, Warning, TEXT("[Execute] It is necessary to set up the target before execution."));
		return;
	}

	

	URGUnitAnimInstance* UnitAnimInstance = Cast<URGUnitAnimInstance>(ControlledUnit->GetMesh()->GetAnimInstance());
	if(UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(true);

	FVector DirectionToTarget = (Target->GetActorLocation() - ControlledUnit->GetActorLocation()).GetSafeNormal();
	FRotator RotationToTarget = FRotator(ControlledUnit->GetActorRotation().Pitch, DirectionToTarget.Rotation().Yaw, DirectionToTarget.Rotation().Roll);
	ControlledUnit->SetActorRotation(RotationToTarget);
	
	ControlledUnit->GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &URGAttackAction::ExecuteAttack, AttackSpeed, true);
}

void URGAttackAction::ExecuteAttack()
{
	if (!Target || Target->IsPendingKill() || !ControlledUnit || ControlledUnit->IsPendingKill())
	{
		StopAttack();
		return;
	}
	
	float DistanceToTarget = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
	if(DistanceToTarget > AttackRange)
	{
		StopAttack();
		return;
	}

	if(Target->GetClass()->ImplementsInterface(URGAttackable::StaticClass()))
	{
		IRGAttackable* AttackableActor = Cast<IRGAttackable>(Target);
		AttackableActor->ReceiveDamage(DamageAmount, ControlledUnit);
	}
}

void URGAttackAction::StopAttack()
{
	if(AttackTimerHandle.IsValid())
		if(GetWorld())
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	
	URGUnitAnimInstance* UnitAnimInstance = Cast<URGUnitAnimInstance>(ControlledUnit->GetMesh()->GetAnimInstance());
	if(UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(false);
	
	OnActionCompletedDelegate().Broadcast();
}