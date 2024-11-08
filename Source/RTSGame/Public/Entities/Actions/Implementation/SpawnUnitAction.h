// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/BaseAction.h"
#include "SpawnUnitAction.generated.h"

class ARGPlayerPawn;
class ARGBuildingBase;

UCLASS()
class RTSGAME_API USpawnUnitAction : public UBaseAction
{
	GENERATED_BODY()

  public:
	virtual void Execute() override;
	virtual void Cancel() override;
	
	void InitializeAction(const TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* PlayerPawn);

  private:
	TSubclassOf<ACharacter> UnitClass;
	UPROPERTY()
	ARGBuildingBase* TargetBuilding;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
