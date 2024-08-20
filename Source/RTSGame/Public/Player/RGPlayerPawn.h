// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RGPlayerPawn.generated.h"

class URGPlayerCameraComponent;
class ARGUnitBase;

UCLASS()
class RTSGAME_API ARGPlayerPawn : public APawn
{
	GENERATED_BODY()

  public:
	ARGPlayerPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	URGPlayerCameraComponent* PlayerCameraComponent;

  private:
	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	UPROPERTY()
	TArray<AActor*> SelectedEntities;
};
