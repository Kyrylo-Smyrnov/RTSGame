// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RGPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMouseWheelInput, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FLeftMouseButtonInputPressed, FVector2D);
DECLARE_MULTICAST_DELEGATE(FLeftMouseButtonInputPressedUninteractable)
DECLARE_MULTICAST_DELEGATE(FLeftMouseButtonInputReleased)

UCLASS()
class RTSGAME_API ARGPlayerController : public APlayerController
{
	GENERATED_BODY()

  public:
	ARGPlayerController();
	virtual void SetupInputComponent() override;

	FMouseWheelInput MouseWheelInput;
	FLeftMouseButtonInputPressed LeftMouseButtonInputPressed;
	FLeftMouseButtonInputPressedUninteractable LeftMouseButtonInputPressedUninteractable;
	FLeftMouseButtonInputReleased LeftMouseButtonInputReleased;

  protected:
	virtual void BeginPlay() override;

  private:
	void OnMouseWheelInput(float Amount);
	void OnLeftMouseButtonInputPressed();
	void OnLeftMouseButtonInputReleased();
};
