// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGPlayerSelectionWidget.generated.h"

class ARGPlayerController;
class UImage;
class UGridPanel;

UCLASS()
class RTSGAME_API URGPlayerSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	void UpdateWidget(TArray<AActor*> SelectedEntities);

  protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* SelectionGrid;

	TArray<UImage*> EntitiesIcons;

  private:
	void InitializeWidget();

	UPROPERTY()
	ARGPlayerController* PlayerController;
};
