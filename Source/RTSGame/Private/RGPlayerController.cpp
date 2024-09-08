// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "RGPlayerController.h"

ARGPlayerController::ARGPlayerController()
{
	bEnableClickEvents = true;
	ClickEventKeys.AddUnique(EKeys::LeftMouseButton);
	ClickEventKeys.AddUnique(EKeys::RightMouseButton);
	DefaultClickTraceChannel = ECC_GameTraceChannel1;
}

void ARGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MouseWheelInput", this, &ARGPlayerController::OnMouseWheelInput);
	
	InputComponent->BindAction("LeftMouseButtonInput", IE_Pressed, this,
							   &ARGPlayerController::OnLeftMouseButtonInputPressed);
	InputComponent->BindAction("LeftMouseButtonInput", IE_Released, this,
							   &ARGPlayerController::OnLeftMouseButtonInputReleased);
	InputComponent->BindAction("RightMouseButtonInput", IE_Pressed, this,
							   &ARGPlayerController::OnRightMouseButtonInputPressed);
}

void ARGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
}

void ARGPlayerController::OnMouseWheelInput(float Amount)
{
	MouseWheelInput.Broadcast(Amount);
}

void ARGPlayerController::OnLeftMouseButtonInputPressed()
{
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	LeftMouseButtonInputPressed.Broadcast(MousePosition);

	FHitResult HitResult;
	if (!GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
		LeftMouseButtonInputPressedUninteractable.Broadcast();
}

void ARGPlayerController::OnLeftMouseButtonInputReleased()
{
	LeftMouseButtonInputReleased.Broadcast();
}

void ARGPlayerController::OnRightMouseButtonInputPressed()
{
	FHitResult HitResult;
	if(!GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
		RightMouseButtonInputPressedUninteractable.Broadcast();
}
