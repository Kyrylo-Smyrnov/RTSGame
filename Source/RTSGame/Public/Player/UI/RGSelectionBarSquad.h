// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGSelectionBarSquad.generated.h"

class ARGPlayerController;
class UImage;
class UGridPanel;

UCLASS()
class RTSGAME_API URGSelectionBarSquad : public UUserWidget
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
};
