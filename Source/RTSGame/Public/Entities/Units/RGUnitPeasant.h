// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Units/RGUnitBase.h"
#include "RGUnitPeasant.generated.h"

class ARGBuildingTownHall;

UCLASS()
class RTSGAME_API ARGUnitPeasant : public ARGUnitBase
{
	GENERATED_BODY()

  public:
	ARGUnitPeasant();

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void AddCarryingWood(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void PutCarryingResources();

	bool GetIsCarryingResources() const;

  protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void InitializeActions() override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Objects")
	TSubclassOf<ARGBuildingTownHall> BuildingTownHallBlueprintClass;

	int32 CarryingWood;
};