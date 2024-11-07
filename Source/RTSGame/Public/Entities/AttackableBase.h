// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackableBase.generated.h"

UCLASS()
class RTSGAME_API AAttackableBase : public AActor
{
	GENERATED_BODY()

  public:
	AAttackableBase() = default;
	AAttackableBase(int32 InHealth);

	virtual void ReceiveDamage(float DamageAmount, AActor* DamageCauser);

  protected:
	virtual void BeginPlay() override;
	
	void StartDecalBlinking();

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UDecalComponent* DecalComponent;

private:
	void BlinkDecal();
	void StopDecalBlinking();

	int32 Health;
	
	FTimerHandle DecalBlinkTimerHandle;
	bool bIsDecalVisible;
};
