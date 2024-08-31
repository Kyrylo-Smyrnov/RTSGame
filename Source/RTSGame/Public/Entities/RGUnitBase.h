// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Actionable.h"
#include "CoreMinimal.h"
#include "EntitiesImportance.h"
#include "GameFramework/Character.h"
#include "RGUnitBase.generated.h"

class ARGPlayerController;
class UDecalComponent;

UCLASS()
class RTSGAME_API ARGUnitBase : public ACharacter, public IActionable
{
	GENERATED_BODY()

  public:
	ARGUnitBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintCallable)
	bool IsSelected() const;
	void SetSelected(bool bIsSelected);

	int32 GetImportance() const;

	virtual TArray<FActionData> GetAvailableActions_Implementation() const override;
	virtual void PerformAction_Implementation(const FName& ActionName) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;

	EFEntitiesImportance UnitImportance;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
	
	bool bIsSelected = false;
};
