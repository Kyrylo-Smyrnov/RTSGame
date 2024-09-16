// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "RGUnitAIController.generated.h"

class ARGPlayerController;

UCLASS()
class RTSGAME_API ARGUnitAIController : public AAIController
{
	GENERATED_BODY()

  protected:
	virtual void BeginPlay() override;
	void HandleRightMouseButtonInputPressedUninteractable();

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;

	const FName BLACKBOARD_KEY_UNITSTATE = "UnitState";
	const FName BLACKBOARD_KEY_TARGETLOCATIONTOMOVE = "TargetLocationToMove";
};
