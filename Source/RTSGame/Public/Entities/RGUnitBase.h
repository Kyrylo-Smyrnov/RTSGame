// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "EntitiesImportance.h"
#include "GameFramework/Character.h"
#include "RGUnitBase.generated.h"

class ARGPlayerController;
class UDecalComponent;

UCLASS()
class RTSGAME_API ARGUnitBase : public ACharacter
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

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;

	// TEST PURPOSE, TO REMOVE
	EFEntitiesImportance UnitImportance = Unit;
	bool bIsSelected = false;
};
