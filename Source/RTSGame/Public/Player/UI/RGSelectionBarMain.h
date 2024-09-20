// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGSelectionBarMain.generated.h"

class ARGPlayerController;
class URGSelectionBarSquad;
class URGSelectionBarQueue;

UCLASS()
class RTSGAME_API URGSelectionBarMain : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	void UpdateWidget(TArray<AActor*> SelectedEntities);

  protected:
	UPROPERTY(meta = (BindWidget))
	class URGSelectionBarSquad* SelectionBarSquad;
	UPROPERTY(meta = (BindWidget))
	class URGSelectionBarQueue* SelectionBarQueue;

  private:
	UPROPERTY()
	ARGPlayerController* PlayerController;
};
