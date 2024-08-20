// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RGUnitBase.generated.h"

class UDecalComponent;

UCLASS()
class RTSGAME_API ARGUnitBase : public ACharacter
{
	GENERATED_BODY()

  public:
	ARGUnitBase();
	virtual void Tick(float DeltaTime) override;

	void SetSelected(bool bIsSelected);

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;

  private:
	bool bIsSelected = false;
};
