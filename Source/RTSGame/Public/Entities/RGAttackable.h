// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGAttackable.generated.h"

UINTERFACE(MinimalAPI)
class URGAttackable : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IRGAttackable
{
	GENERATED_BODY()

  public:
	virtual void ReceiveDamage(float DamageAmount, AActor* DamageCauser) = 0;
};