// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "RGUnitAnimInstance.generated.h"

UENUM()
enum class EUnitClass : uint8
{
	UC_Peasant UMETA(DisplayName = "Peasant")
};

UCLASS()
class RTSGAME_API URGUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
  public:
	void SetIsAttacking(bool IsAttacking);

  protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	EUnitClass UnitClass;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Actions")
	bool bIsAttacking;
};
