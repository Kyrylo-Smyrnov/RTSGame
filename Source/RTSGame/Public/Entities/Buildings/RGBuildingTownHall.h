// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "RGBuildingTownHall.generated.h"

class ARGUnitPeasant;

UCLASS()
class RTSGAME_API ARGBuildingTownHall : public ARGBuildingBase
{
	GENERATED_BODY()

  public:
	ARGBuildingTownHall();

  protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed) override;
	virtual void InitializeActions() override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Objects")
	TSubclassOf<ARGUnitPeasant> UnitPeasantBlueprintClass;
};
