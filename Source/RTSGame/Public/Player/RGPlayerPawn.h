// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/RGAction.h"
#include "GameFramework/Pawn.h"
#include "RGPlayerPawn.generated.h"

class ARGPlayerController;
class URGPlayerCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedEntitiesChanged, TArray<AActor*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMostImportantEntityChanged, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerResourcesChanged, int32);

UCLASS()
class RTSGAME_API ARGPlayerPawn : public APawn
{
	GENERATED_BODY()

  public:
	ARGPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandleLeftMouseButtonInputPressedUninteractable();

	UFUNCTION(BlueprintCallable)
	AActor* GetMostImportantEntity() const;
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetSelectedEntities() const;

	void AddEntitiesToContolled(AActor* Entity);
	void RemoveEntityFromControlled(AActor* Entity);
	void AddEntitiesToSelected(AActor* Entity);
	void AddEntitiesToSelected(TArray<AActor*> Entities);
	void RemoveEntityFromSelected(AActor* Entity);
	void ClearSelectedEntities();
	bool IsEntitySelected(AActor* Entity) const;

	void AddPlayerResources(int32 Amount);
	int32 GetPlayerResources() const;

	void SetAwaitingAction(IRGAction* Action);

	FOnSelectedEntitiesChanged OnSelectedEntitiesChanged;
	FOnMostImportantEntityChanged OnMostImportantEntityChanged;
	FOnPlayerResourcesChanged OnPlayerResourcesChanged;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	URGPlayerCameraComponent* PlayerCameraComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Entities")
	TArray<AActor*> ControlledEntities;

  private:
	static bool CompareEntityImportance(const AActor& A, const AActor& B);
	void ExecuteActionWithTarget(FVector TargetLocation);

	UPROPERTY()
	ARGPlayerController* PlayerController;

	UPROPERTY()
	TArray<AActor*> SelectedEntities;
	
	IRGAction* AwaitingAction;

	int32 PlayerWoodResource;
	bool bIsAwaitingTarget;
};
