// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/BaseAction.h"
#include "ConstructBuildingAction.generated.h"

class ARGPlayerPawn;

UCLASS()
class RTSGAME_API UConstructBuildingAction : public UBaseAction
{
	GENERATED_BODY()
  public:
	UConstructBuildingAction();

	virtual void Execute() override;
	
	void InitializeAction(TSubclassOf<AActor> InBuildingClass, ARGPlayerPawn* InPlayerPawn);

  private:
	TSubclassOf<AActor> BuildingClass;
	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
};
