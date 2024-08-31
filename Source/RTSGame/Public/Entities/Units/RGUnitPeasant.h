// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/RGUnitBase.h"
#include "RGUnitPeasant.generated.h"

UCLASS()
class RTSGAME_API ARGUnitPeasant : public ARGUnitBase
{
	GENERATED_BODY()

  public:
	ARGUnitPeasant();
	virtual void Tick(float DeltaSeconds) override;

  protected:
	virtual void BeginPlay() override;
	
	virtual TArray<FActionData> GetAvailableActions_Implementation() const override;
	virtual void PerformAction_Implementation(const FName& ActionName) override;
};
