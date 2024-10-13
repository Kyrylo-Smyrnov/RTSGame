// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "RGAction.h"
#include "RGSpawnUnitAction.generated.h"

class ARGPlayerPawn;
class ARGBuildingBase;

UCLASS()
class RTSGAME_API URGSpawnUnitAction : public UObject, public IRGAction
{
	GENERATED_BODY()

  public:
	URGSpawnUnitAction();
	
	void InitializeAction(TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* PlayerPawn);
	virtual void Execute_Implementation() override;

  private:
	TSubclassOf<ACharacter> UnitClass;
	UPROPERTY()
	ARGBuildingBase* TargetBuilding;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
