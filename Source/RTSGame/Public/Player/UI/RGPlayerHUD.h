// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RGPlayerHUD.generated.h"

class UEntityHandlerComponent;
class URGActionGridWidget;
class ARGPlayerController;
class ARGPlayerPawn;

UCLASS()
class RTSGAME_API ARGPlayerHUD : public AHUD
{
	GENERATED_BODY()

  public:
	virtual void DrawHUD() override;

	URGActionGridWidget* GetActionGridWidget() const;

  protected:
	virtual void BeginPlay() override;

  private:
	void HandleLeftMouseButtonInputPressed();
	void HandleLeftMouseButtonInputReleased();

	void DrawSelectionBox();

	UPROPERTY()
	ARGPlayerController* PlayerController;
	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;

	UPROPERTY()
	URGActionGridWidget* ActionGridWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ActionGridWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ResourceWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> SelectionBarMainClass;

	UPROPERTY()
	TArray<AActor*> SelectedEntities;

	FVector2D SelectionBoxStartPoint = FVector2D::ZeroVector;
	FVector2D SelectionBoxEndPoint = FVector2D::ZeroVector;

	const FLinearColor SELECTION_BOX_OUTLINE_COLOR = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
	const float SELECTION_BOX_OUTLINE_THICKNESS = 1.0f;

	bool bIsSelectionBoxDrawn = false;
	bool bIsSelectionBox = false;
};
