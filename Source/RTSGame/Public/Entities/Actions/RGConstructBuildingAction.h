// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "RGAction.h"
#include "CoreMinimal.h"
#include "RGConstructBuildingAction.generated.h"

class ARGPlayerPawn;

UCLASS()
class RTSGAME_API URGConstructBuildingAction : public UObject, public IRGAction
{
	GENERATED_BODY()
  public:
	URGConstructBuildingAction();
	
	void InitializeAction(TSubclassOf<AActor> InBuildingClass, ARGPlayerPawn* InPlayerPawn);
	virtual void Execute_Implementation() override;

  private:
	TSubclassOf<AActor> BuildingClass;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
