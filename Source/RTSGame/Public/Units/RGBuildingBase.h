// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RGBuildingBase.generated.h"

class ARGPlayerController;
class UDecalComponent;
class UStaticMeshComponent;

UCLASS()
class RTSGAME_API ARGBuildingBase : public APawn
{
	GENERATED_BODY()

  public:
	ARGBuildingBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	bool IsSeleted() const;
	void SetSelected(bool bIsBuildingSelected);

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
	
	bool bIsSelected = false;
};
