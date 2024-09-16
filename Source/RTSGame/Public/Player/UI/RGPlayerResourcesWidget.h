// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGPlayerResourcesWidget.generated.h"

class UImage;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class RTSGAME_API URGPlayerResourcesWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void NativeConstruct() override;
	void UpdateWidget(int32 WoodAmount);

  protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ResourcesHorizontalBox;

	UPROPERTY()
	UTextBlock* WoodCountText;
	UPROPERTY()
	UImage* WoodIcon;

  private:
	void InitializeWidget();
};
