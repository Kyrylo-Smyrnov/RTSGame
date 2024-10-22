// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Actions/Implementation/AttackAction.h"
#include "Entities/RGAttackable.h"
#include "Entities/Units/Animation/RGUnitAnimInstance.h"
#include "Entities/Units/RGUnitBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGAttackAction, All, All)

void UAttackAction::InitializeAction(ARGUnitBase* Unit)
{
	ControlledUnit = Unit;
	UnitAnimInstance = Cast<URGUnitAnimInstance>(ControlledUnit->GetMesh()->GetAnimInstance());

	// DamageAmount = Unit->GetStatDamage();
	// AttackSpeed = Unit->GetStatAttackSpeed();
	// AttackRange = Unit->GetStatAttackRange();
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
		UE_LOG(LogRGAttackAction, Warning, TEXT("[Execute] It is necessary to initialize the action before execution."));
		return;
	}

	if (!Target)
	{
		UE_LOG(LogRGAttackAction, Warning, TEXT("[Execute] It is necessary to set up the target before execution."));
		return;
	}

	if (UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(true);

	FVector DirectionToTarget = (Target->GetActorLocation() - ControlledUnit->GetActorLocation()).GetSafeNormal();
	FRotator RotationToTarget = FRotator(ControlledUnit->GetActorRotation().Pitch, DirectionToTarget.Rotation().Yaw, DirectionToTarget.Rotation().Roll);
	ControlledUnit->SetActorRotation(RotationToTarget);

	ControlledUnit->GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UAttackAction::ExecuteAttack, AttackSpeed, true);
}

void UAttackAction::Cancel_Implementation()
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

	if (Target->GetClass()->ImplementsInterface(URGAttackable::StaticClass()))
	{
		IRGAttackable* AttackableActor = Cast<IRGAttackable>(Target);
		AttackableActor->ReceiveDamage(DamageAmount, ControlledUnit);
	}
}

void UAttackAction::StopAttack(AActor* DestroyedActor)
{
	if (Target)
		Target->OnDestroyed.RemoveDynamic(this, &UAttackAction::StopAttack);

	if (UnitAnimInstance)
		UnitAnimInstance->SetIsAttacking(false);

	if (AttackTimerHandle.IsValid())
		if (GetWorld())
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	OnActionCompletedDelegate().Broadcast();
}