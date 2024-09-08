// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitAnimInstance.h"
#include "Entities/Units/RGUnitPeasant.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGUnitAnimInstance, All, All);

void URGUnitAnimInstance::SetIsAttacking(bool IsAttacking)
{
	bIsAttacking = IsAttacking;
}

void URGUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	bIsAttacking = false;

	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		if (Cast<ARGUnitPeasant>(Pawn))
			UnitClass = EUnitClass::UC_Peasant;
	}
	else
	{
		UE_LOG(LogRGUnitAnimInstance, Warning, TEXT("[NativeInitializeAnimation]: !Pawn"));
	}
}

void URGUnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		Speed = Pawn->GetVelocity().Size();
	}
	else
	{
		UE_LOG(LogRGUnitAnimInstance, Warning, TEXT("[NativeUpdateAnimation]: !Pawn"));
	}
}