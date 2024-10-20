// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Entities/Actions/RGAction.h"
#include "RGActionGridWidget.generated.h"

class UBaseAction;
class ARGPlayerController;
class UButton;
class UGridPanel;
class UImage;

UCLASS()
class RTSGAME_API URGActionGridWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void NativeConstruct() override;
	void UpdateWidget(AActor* MostImportantEntity);

  protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ActionGrid;

	TMap<UButton*, UBaseAction*> ActionButtons;
	TArray<UImage*> ActionIcons;

  private:
	void InitializeWidget();
	UFUNCTION()
	void HandleButtonClick();

	UPROPERTY()
	ARGPlayerController* PlayerController;
};