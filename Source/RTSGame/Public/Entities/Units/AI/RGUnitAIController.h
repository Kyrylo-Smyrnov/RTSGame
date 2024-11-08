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
	void HandleRightMouseButtonInputPressed();

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
};
