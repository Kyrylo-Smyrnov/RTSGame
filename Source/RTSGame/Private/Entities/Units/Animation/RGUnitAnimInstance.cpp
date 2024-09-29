// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/Animation/RGUnitAnimInstance.h"
#include "Entities/Units/RGUnitPeasant.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGUnitAnimInstance, All, All);

void URGUnitAnimInstance::SetIsAttacking(bool IsAttacking)
{
	bIsAttacking = IsAttacking;
}

void URGUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn)
	{
		UE_LOG(LogRGUnitAnimInstance, Warning, TEXT("[NativeInitializeAnimation]: !Pawn"));
		return;
	}

	if (Cast<ARGUnitPeasant>(Pawn))
		UnitClass = EUnitClass::UC_Peasant;

	bIsAttacking = false;
}

void URGUnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn)
	{
		UE_LOG(LogRGUnitAnimInstance, Warning, TEXT("[NativeInitializeAnimation]: !Pawn"));
		return;
	}

	Speed = Pawn->GetVelocity().Size();
}