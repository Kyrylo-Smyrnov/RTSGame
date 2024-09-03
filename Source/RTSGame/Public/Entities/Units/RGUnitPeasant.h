// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/RGUnitBase.h"
#include "RGUnitPeasant.generated.h"

class ARGBuildingTownHall;

UCLASS()
class RTSGAME_API ARGUnitPeasant : public ARGUnitBase
{
	GENERATED_BODY()

  public:
	ARGUnitPeasant();
	virtual void Tick(float DeltaSeconds) override;

	virtual void PerformAction_Implementation(const FName& ActionName) override;

  protected:
	virtual void BeginPlay() override;

	virtual TArray<FActionData> GetAvailableActions_Implementation() const override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TSubclassOf<ARGBuildingTownHall> BuildingTownHallBlueprintClass;
};
