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
	USpawnUnitAction();
	
	void InitializeAction(TSubclassOf<ACharacter> InUnitClass, ARGBuildingBase* InTargetBuilding, ARGPlayerPawn* PlayerPawn);
	virtual void Execute_Implementation() override;
	virtual void Cancel_Implementation() override;

  private:
	TSubclassOf<ACharacter> UnitClass;
	UPROPERTY()
	ARGBuildingBase* TargetBuilding;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
