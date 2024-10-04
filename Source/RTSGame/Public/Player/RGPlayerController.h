// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "RGPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMouseWheelInput, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FLeftMouseButtonInputPressed, FVector2D);
DECLARE_MULTICAST_DELEGATE(FLeftMouseButtonInputPressedUninteractable);
DECLARE_MULTICAST_DELEGATE(FLeftMouseButtonInputReleased);

DECLARE_MULTICAST_DELEGATE(FRightMouseButtonInputPressed);

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
	FRightMouseButtonInputPressed RightMouseButtonInputPressed;

	UNavigationSystemV1* GetNavigationSystem() const;

  protected:
	virtual void BeginPlay() override;

  private:
	void OnMouseWheelInput(float Amount);
	void OnLeftMouseButtonInputPressed();
	void OnLeftMouseButtonInputReleased();
	void OnRightMouseButtonInputPressed();

	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;
};
