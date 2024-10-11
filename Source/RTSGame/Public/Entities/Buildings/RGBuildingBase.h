// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "BehaviorTree/BlackboardComponent.h"
#include "CoreMinimal.h"
#include "Entities/Actions/RGAction.h"
#include "Entities/Actions/RGActionsList.h"
#include "Entities/Actions/RGSpawnUnitAction.h"
#include "Entities/BBKeys.h"
#include "Entities/Buildings/SpawnQueueEntry.h"
#include "Entities/EntitiesImportance.h"
#include "Entities/Units/AI/RGUnitAIController.h"
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

	void AddUnitToSpawnQueue(TSubclassOf<AActor> UnitClass, float SpawnTime);

	bool GetIsSelected() const;
	bool GetIsConstructing() const;
	int32 GetImportance() const;
	UTexture2D* GetSelectionIcon() const;
	TArray<FSpawnQueueEntry>& GetSpawnQueue();
	TArray<IRGAction*>& GetAvailableActions();

	void SetSelected(bool bIsBuildingSelected);
	void SetTimeToConstruct(float Time);

	void SetBuildingPlacementMaterial(const bool IsValidPlacement);
	void SetBuildingMeshMaterials();

	void SetBannerLocation(FVector NewLocation);
	void SetActorToAttackForSpawnedUnit(AActor* NewActorToAttack);

	FOnSpawnQueueChanged OnSpawnQueueChanged;
	FOnSpawnProgressChanged OnSpawnProgressChanged;
	FOnConstructionProgressChanged OnConstructionProgressChanged;

  protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitializeActions();
	UFUNCTION()
	virtual void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

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

	TArray<IRGAction*> AvailableActions;

	EFEntitiesImportance BuildingImportance;

  private:
	bool CheckForOverlap() const;

	void SpawnNextUnit();

	void HandleBuildingPlacing();
	void HandleBuildingConstructing();
	void HandleRightMouseButtonInputPressed();

	UPROPERTY()
	ARGPlayerController* PlayerController;

	UPROPERTY()
	AActor* ActorToAttackForSpawnedUnits;
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

	FVector LastBannerLocation;

	bool bIsSelected;
	bool bIsPlacing;
	bool bIsConstructing;

	float TimeToConstruct;
};
