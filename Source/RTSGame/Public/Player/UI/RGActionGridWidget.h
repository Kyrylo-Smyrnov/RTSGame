// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGActionGridWidget.generated.h"

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
	void UpdateActionButtons(AActor* MostImportantEntity);

  protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* ActionGrid;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI");
	TMap<UButton*, FName> ActionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TArray<UImage*> ActionIcons;

  private:
	void InitializeGrid();
	UFUNCTION()
	void HandleButtonClick();

	UPROPERTY()
	ARGPlayerController* PlayerController;
};