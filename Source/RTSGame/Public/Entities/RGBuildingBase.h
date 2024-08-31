// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "EntitiesImportance.h"
#include "GameFramework/Pawn.h"
#include "RGBuildingBase.generated.h"

class ARGPlayerController;
class UDecalComponent;
class UStaticMeshComponent;

UCLASS()
class RTSGAME_API ARGBuildingBase : public APawn
{
	GENERATED_BODY()

  public:
	ARGBuildingBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	bool IsSeleted() const;
	int32 GetImportance() const;

	void SetSelected(bool bIsBuildingSelected);
	void SetBuildingPlacementMaterial(const bool IsValidPlacement) const;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	EFEntitiesImportance BuildingImportance;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;

	UPROPERTY()
	UMaterialInterface* ValidPlacementMaterial;
	UPROPERTY()
	UMaterialInterface* InValidPlacementMaterial;

	bool bIsSelected = false;
};
