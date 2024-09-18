// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actionable.h"
#include "Entities/RGBuildingBase.h"
#include "RGBuildingTownHall.generated.h"

class ARGUnitPeasant;

UCLASS()
class RTSGAME_API ARGBuildingTownHall : public ARGBuildingBase, public IActionable
{
	GENERATED_BODY()

  public:
	ARGBuildingTownHall();
	virtual void Tick(float DeltaSeconds) override;

	virtual void PerformAction_Implementation(const FName& ActionName) override;

  protected:
	virtual void BeginPlay() override;

	virtual TArray<FActionData> GetAvailableActions_Implementation() const override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TSubclassOf<ARGUnitPeasant> UnitPeasantBlueprintClass;

	const FName ACTION_BUILDPEASANT = "BuildPeasant";
};
