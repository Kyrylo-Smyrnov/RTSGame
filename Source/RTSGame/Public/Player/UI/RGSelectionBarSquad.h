// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGSelectionBarSquad.generated.h"

class UImage;

UCLASS()
class RTSGAME_API URGSelectionBarSquad : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	void UpdateWidget(TArray<AActor*> SelectedEntities);

  protected:
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_0;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_1;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_2;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_3;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_4;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_5;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_6;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_7;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_8;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_9;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_10;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_11;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_12;
	UPROPERTY(meta = (BindWidget))
	UImage* EntityIcon_13;

  private:
	void InitializeWidget();
	
	TArray<UImage*> EntitiesIcons;
};
