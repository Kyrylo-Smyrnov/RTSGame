// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/Actions/ActionsList.h"
#include "Entities/Actions/BaseAction.h"
#include "Entities/EntitiesImportance.h"
#include "GameFramework/Character.h"
#include "RGUnitBase.generated.h"

class UEntityHandlerComponent;
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
	TArray<UBaseAction*> GetAvailableActions() const;

	void SetSelected(bool bIsSelected);

	void AddActionToQueue(UBaseAction* Action) const;
	bool CanPerformAction(UBaseAction* Action) const;
	void ClearActionQueue() const;

  protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	virtual void InitializeActions();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* SelectionCircleDecal;
	
	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SelectionIcon;

	UPROPERTY()
	TArray<UBaseAction*> AvailableActions;

	EFEntitiesImportance UnitImportance;

  private:
	UPROPERTY()
	class UActionQueue* ActionQueue;

	bool bIsSelected;
};
