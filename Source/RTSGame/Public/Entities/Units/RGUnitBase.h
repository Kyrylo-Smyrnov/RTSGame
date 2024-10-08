// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Entities/EntitiesImportance.h"
#include "GameFramework/Character.h"
#include "Player/RGPlayerPawn.h"
#include "RGUnitBase.generated.h"

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

	void SetSelected(bool bIsSelected);

  protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed);

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
