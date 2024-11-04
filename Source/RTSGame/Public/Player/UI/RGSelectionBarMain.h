// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RGSelectionBarMain.generated.h"

struct FSpawnQueueEntry;
class UEntityHandlerComponent;
class ARGBuildingBase;
class URGSelectionBarSquad;
class URGSelectionBarQueue;

UENUM()
enum ECurrentWidgetState
{
	SelectionBarSquadState,
	SelectionBarQueueState,
	SelectionBarStatState
};

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
	void HandleSpawnQueueChange(TArray<FSpawnQueueEntry>& SpawnQueue);

	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;
	UPROPERTY()
	ARGBuildingBase* SelectedBuilding;

	ECurrentWidgetState CurrentState;
};
