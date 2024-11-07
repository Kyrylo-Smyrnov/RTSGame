// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/AttackableBase.h"
#include "ResourceBase.generated.h"

class UEntityHandlerComponent;

UCLASS()
class RTSGAME_API AResourceBase : public AAttackableBase
{
	GENERATED_BODY()

  public:
	AResourceBase();

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void ReceiveDamage(float DamageAmount, AActor* DamageCauser) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

  private:
	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;
};
