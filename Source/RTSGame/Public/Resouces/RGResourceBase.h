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

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
