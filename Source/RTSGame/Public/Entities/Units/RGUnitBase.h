// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/Actionable.h"
#include "Entities/EntitiesImportance.h"
#include "GameFramework/Character.h"
#include "RGUnitBase.generated.h"

class ARGPlayerPawn;
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

	void SetSelected(bool bIsSelected);

	// UFUNCTION(BlueprintCallable)
	bool IsSelected() const; // TODO: Change to GetIsSelected.
	int32 GetImportance() const;
	UTexture2D* GetSelectionIcon() const;

	virtual void PerformAction_Implementation(const FName& ActionName) override;

  protected:
	virtual void BeginPlay() override;
	virtual TArray<FActionData> GetAvailableActions_Implementation() const override;

	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SelectionIcon;

	EFEntitiesImportance UnitImportance;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;

	bool bIsSelected;

	const FName ACTION_ATTACK = "AttackAction";
	const FName ACTION_HOLDATTACK = "HoldAction";
	const FName ACTION_MOVE = "MoveAction";
	const FName ACTION_MOVEATTACK = "MoveAttackAction";
};