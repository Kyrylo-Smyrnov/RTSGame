// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGActionGridWidget.generated.h"

class UEntityHandlerComponent;
class UBaseAction;
class UButton;
class UImage;

UCLASS()
class RTSGAME_API URGActionGridWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	void UpdateWidget(AActor* MostImportantEntity);

  protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_0;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_0;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_1;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_1;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_2;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_2;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_3;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_3;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_4;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_4;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_5;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_5;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_6;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_6;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_7;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_7;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_8;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_8;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_9;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_9;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_10;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_10;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton_11;
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage_11;

  private:
	void InitializeWidget();
	UFUNCTION()
	void HandleButtonClick();

	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;

	TMap<UButton*, UBaseAction*> ActionButtons;
	TArray<UImage*> ActionIcons;
};