// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RGPlayerPawn.generated.h"

class ARGPlayerController;
class URGPlayerCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedEntitiesChanged, AActor*)

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
	void AddEntityToSelected(AActor* Entity);
	void RemoveEntityFromSelected(AActor* Entity);
	void ClearSelectedEntities();
	bool IsEntitySelected(AActor* Entity) const;

	FOnSelectedEntitiesChanged OnSelectedEntitiesChanged;
	
  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	URGPlayerCameraComponent* PlayerCameraComponent;

  private:
	static bool CompareEntityImportance(const AActor& A, const AActor& B);
	
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	TArray<AActor*> SelectedEntities;
};
