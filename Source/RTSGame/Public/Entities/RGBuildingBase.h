// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Buildings/SpawnQueueEntry.h"
#include "CoreMinimal.h"
#include "EntitiesImportance.h"
#include "GameFramework/Pawn.h"
#include "RGBuildingBase.generated.h"

class ARGPlayerController;
class ARGPlayerPawn;
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

	void AddUnitToSpawnQueue(TSubclassOf<AActor> UnitClass, float SpawnTime);

	bool IsSelected() const;
	void SetSelected(bool bIsBuildingSelected);

	int32 GetImportance() const;
	UTexture2D* GetSelectionIcon() const;

	void SetBuildingPlacementMaterial(const bool IsValidPlacement);
	void SetBuildingMeshMaterials();

  protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SelectionIcon;

	EFEntitiesImportance BuildingImportance;

  private:
	void SpawnUnitFromQueue(int32 QueueIndex);
	void SpawnNextUnit();
	void HandleBuildingConstructing();

	UPROPERTY()
	ARGPlayerController* PlayerController;

	FTimerHandle SpawnTimerHandle;
	TArray<FSpawnQueueEntry> SpawnQueue;
	float RemainingSpawnTime;
	bool bIsSpawning;

	UPROPERTY()
	UMaterialInterface* ValidPlacementMaterial;
	UPROPERTY()
	UMaterialInterface* InValidPlacementMaterial;
	UPROPERTY()
	TArray<UMaterialInterface*> BuildingMeshMaterials;

	bool CheckForOverlap();

	bool bIsSelected;
	bool bIsConstructing;
};
