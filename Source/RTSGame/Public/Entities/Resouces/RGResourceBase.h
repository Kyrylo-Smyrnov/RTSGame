// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/RGAttackable.h"
#include "GameFramework/Actor.h"
#include "RGResourceBase.generated.h"

class ARGPlayerController;
class ARGPlayerPawn;

UCLASS()
class RTSGAME_API ARGResourceBase : public AActor, public IRGAttackable
{
	GENERATED_BODY()

  public:
	ARGResourceBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void ReceiveDamage(float DamageAmount, AActor* DamageCauser) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* DecalComponent;
	
	int32 ResourceAmount;

  private:
	void BlinkDecal();
	
	FTimerHandle BlinkTimerHandle;
	bool bIsDecalVisible = false;
	
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
