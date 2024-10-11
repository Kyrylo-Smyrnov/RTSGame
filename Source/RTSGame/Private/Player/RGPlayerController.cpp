// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerController, All, All);

ARGPlayerController::ARGPlayerController()
	: NavigationSystem(nullptr)
{
	bEnableClickEvents = true;
	ClickEventKeys.AddUnique(EKeys::LeftMouseButton);
	ClickEventKeys.AddUnique(EKeys::RightMouseButton);
	DefaultClickTraceChannel = ECC_GameTraceChannel1;
}

void ARGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		UE_LOG(LogRGPlayerController, Warning, TEXT("[SetupInputComponent] InputComponent is nullptr."));
		return;
	}

	InputComponent->BindAxis("MouseWheelInput", this, &ARGPlayerController::OnMouseWheelInput);

	InputComponent->BindAction("LeftMouseButtonInput", IE_Pressed, this, &ARGPlayerController::OnLeftMouseButtonInputPressed);
	InputComponent->BindAction("LeftMouseButtonInput", IE_Released, this, &ARGPlayerController::OnLeftMouseButtonInputReleased);
	InputComponent->BindAction("RightMouseButtonInput", IE_Pressed, this, &ARGPlayerController::OnRightMouseButtonInputPressed);
}

UNavigationSystemV1* ARGPlayerController::GetNavigationSystem() const
{
	return NavigationSystem;
}

void ARGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);

	NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavigationSystem)
		UE_LOG(LogRGPlayerController, Warning, TEXT("[Constructor] NavigationSystem is nullptr."))
}

void ARGPlayerController::OnMouseWheelInput(float Amount)
{
	if (MouseWheelInput.IsBound())
		MouseWheelInput.Broadcast(Amount);
}

void ARGPlayerController::OnLeftMouseButtonInputPressed()
{
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	if (LeftMouseButtonInputPressed.IsBound())
		LeftMouseButtonInputPressed.Broadcast(MousePosition);

	FHitResult HitResult;
	if (!GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult) && LeftMouseButtonInputPressedUninteractable.IsBound())
		LeftMouseButtonInputPressedUninteractable.Broadcast();
}

void ARGPlayerController::OnLeftMouseButtonInputReleased()
{
	if (LeftMouseButtonInputReleased.IsBound())
		LeftMouseButtonInputReleased.Broadcast();
}

void ARGPlayerController::OnRightMouseButtonInputPressed()
{
	if (RightMouseButtonInputPressed.IsBound())
		RightMouseButtonInputPressed.Broadcast();
}
