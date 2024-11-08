// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/ActionsList.h"
#include "Entities/EntitiesImportance.h"
#include "Entities/Actions/BaseAction.h"
#include "GameFramework/Character.h"
#include "Player/RGPlayerPawn.h"
#include "RGUnitBase.generated.h"

class URGMoveToAction;
class ARGPlayerController;
class UDecalComponent;

UCLASS()
class RTSGAME_API ARGUnitBase : public ACharacter
{
	GENERATED_BODY()

  public:
	ARGUnitBase();

	bool GetIsSelected() const;
	int32 GetImportance() const;
	UTexture2D* GetSelectionIcon() const;
	TArray<UBaseAction*> GetAvailableActions();

	void SetSelected(bool bIsSelected);

	void AddActionToQueue(UBaseAction* Action) const;
	void ClearActionQueue() const;

	bool CanPerformAction(UBaseAction* Action);

  protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void InitializeActions();

	UPROPERTY()
	ARGPlayerPawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SelectionIcon;

	UPROPERTY()
	TArray<UBaseAction*> AvailableActions;

	EFEntitiesImportance UnitImportance;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	class UActionQueue* ActionQueue;

	bool bIsSelected;
};
