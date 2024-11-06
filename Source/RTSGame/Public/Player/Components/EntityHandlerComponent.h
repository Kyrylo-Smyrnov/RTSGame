// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EntityHandlerComponent.generated.h"

class UBaseAction;
class ARGPlayerController;
class ARGUnitBase;
class ARGBuildingBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedEntitiesChanged, TArray<AActor*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMostImportantEntityChanged, AActor*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RTSGAME_API UEntityHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UEntityHandlerComponent();

	bool IsEntitySelected(AActor* Entity) const;

	void AddUnitToSelected(ARGUnitBase* Unit);
	void AddBuildingToSelected(ARGBuildingBase* Building);
	void AddEntitiesToSelected(TArray<AActor*> Entities);

	void RemoveUnitFromSelected(ARGUnitBase* Unit);
	void RemoveBuildingFromSelected(ARGBuildingBase* Building);
	void ClearSelectedEntities();

	AActor* GetMostImportantEntity() const;
	TArray<AActor*> GetSelectedEntities() const;

	void SetAwaitingAction(UBaseAction* Action);
	void ExecuteAction(UBaseAction* Action);

	FOnSelectedEntitiesChanged OnSelectedEntitiesChanged;
	FOnMostImportantEntityChanged OnMostImportantEntityChanged;

  protected:
	virtual void BeginPlay() override;

  private:
	void HandleLeftMouseButtonInputPressed();
	void ExecuteActionWithTarget(TVariant<FVector, AActor*> TargetVariant, bool bMustBeEnqueued);

	void IdentifyMostImportantEntity();
	static bool CompareEntityImportance(const AActor& A, const AActor& B);

	UPROPERTY()
	ARGPlayerController* PlayerController;

	UPROPERTY()
	AActor* MostImportantEntity;
	TArray<ARGUnitBase*> SelectedUnits;
	TArray<ARGBuildingBase*> SelectedBuildings;

	UPROPERTY()
	UBaseAction* AwaitingAction;
};
