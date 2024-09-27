// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Buildings/SpawnQueueEntry.h"
#include "CoreMinimal.h"
#include "EntitiesImportance.h"
#include "GameFramework/Pawn.h"
#include "RGBuildingBase.generated.h"

class ARGBuildingBanner;
class ARGPlayerController;
class ARGPlayerPawn;
class UDecalComponent;
class UStaticMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpawnQueueChanged, TArray<FSpawnQueueEntry>& SpawnQueue);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpawnProgressChanged, float Progress);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnConstructionProgressChanged, float Progress);

UCLASS()
class RTSGAME_API ARGBuildingBase : public APawn
{
	GENERATED_BODY()

  public:
	ARGBuildingBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	void AddUnitToSpawnQueue(TSubclassOf<AActor> UnitClass, float SpawnTime);

	int32 GetImportance() const;
	UTexture2D* GetSelectionIcon() const;
	TArray<FSpawnQueueEntry>& GetSpawnQueue();

	bool GetIsConstructing() const;
	bool IsSelected() const; // TODO: Change to GetIsSelected.

	void SetBuildingPlacementMaterial(const bool IsValidPlacement);
	void SetBuildingMeshMaterials();

	void SetSelected(bool bIsBuildingSelected);
	void SetTimeToConstruct(float Time);

	FOnSpawnQueueChanged OnSpawnQueueChanged;
	FOnSpawnProgressChanged OnSpawnProgressChanged;
	FOnConstructionProgressChanged OnConstructionProgressChanged;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponentCurrent;

	UPROPERTY(EditDefaultsOnly, Category = "Objects")
	TSubclassOf<ARGBuildingBanner> BuildingBannerClass;
	UPROPERTY()
	ARGBuildingBanner* BuildingBanner;

	UPROPERTY(EditDefaultsOnly, Category = "Construction")
	UStaticMesh* StaticMeshConstructionPhase1;
	UPROPERTY(EditDefaultsOnly, Category = "Construction")
	UStaticMesh* StaticMeshConstructionPhase2;
	UPROPERTY(EditDefaultsOnly, Category = "Construction")
	UStaticMesh* StaticMeshConstructionPhase3;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SelectionIcon;

	EFEntitiesImportance BuildingImportance;

  private:
	bool CheckForOverlap();

	void SpawnNextUnit();

	void HandleBuildingPlacing();
	void HandleBuildingConstructing();
	void HandleRightMouseButtonInputPressedUninteractable();

	UPROPERTY()
	ARGPlayerController* PlayerController;

	FVector LastBannerLocation;

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

	bool bIsSelected;
	bool bIsPlacing;
	bool bIsConstructing;

	float TimeToConstruct;
};
