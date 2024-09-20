// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGSelectionBarQueue.generated.h"

class ARGBuildingBase;
class UImage;
class UProgressBar;
class UVerticalBox;

UCLASS()
class RTSGAME_API URGSelectionBarQueue : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	void UpdateWidget(ARGBuildingBase* Building);

  protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* MainVerticalBox;
	UPROPERTY()
	UProgressBar* SpawnProgressBar;
	UPROPERTY()
	UImage* CurrentUnitIcon;
	TArray<UImage*> UnitsInQueueIcons;

  private:
	void InitializeWidget();
};
