// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGResourceBase.generated.h"

class ARGPlayerController;
class ARGPlayerPawn;

UCLASS()
class RTSGAME_API ARGResourceBase : public AActor
{
	GENERATED_BODY()

  public:
	ARGResourceBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintCallable)
	void DealDamage(const float Amount);

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 Health;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;

	const FName BLACKBOARD_KEY_UNITSTATE = "UnitState";
	const FName BLACKBOARD_KEY_TARGETLOCATIONTOMOVE = "TargetLocationToMove";
	const FName BLACKBOARD_KEY_TARGETACTORTOATTACK = "TargetActorToAttack";
};