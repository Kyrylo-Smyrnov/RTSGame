// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerCameraComponent, All, All);

URGPlayerCameraComponent::URGPlayerCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CameraRootComponent = CreateDefaultSubobject<USceneComponent>("CameraRootComponent");
	CameraRootComponent->SetupAttachment(this);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 2000.0f;
	SpringArmComponent->AddLocalRotation(FRotator(-70.0f, 0.0f, 0.0f));
	SpringArmComponent->SetupAttachment(CameraRootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void URGPlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleCameraMovement();
}

void URGPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
		PlayerController->MouseWheelInput.AddUObject(this, &URGPlayerCameraComponent::HandleMouseWheelInput);
	else
		UE_LOG(LogRGPlayerCameraComponent, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));
}

void URGPlayerCameraComponent::HandleCameraMovement()
{
	if (!CameraRootComponent)
	{
		UE_LOG(LogRGPlayerCameraComponent, Warning, TEXT("[HandleCameraMovement] CameraRootComponent is nullptr."));
		return;
	}

	FVector2D MousePosition = FVector2D::ZeroVector;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	PlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);
	CameraScrollBoundX = ViewportSize.X * SCROLL_BOUNDARY_FRACTION;
	CameraScrollBoundY = ViewportSize.Y * SCROLL_BOUNDARY_FRACTION;

	float MovementX = 0.0f;
	float MovementY = 0.0f;

	if (MousePosition.X < CameraScrollBoundX)
		MovementX = -CameraSpeed * (1.0f - MousePosition.X / CameraScrollBoundX);
	else if (MousePosition.X > (ViewportSize.X - CameraScrollBoundX))
		MovementX = CameraSpeed * ((MousePosition.X - (ViewportSize.X - CameraScrollBoundX)) / CameraScrollBoundX);

	if (MousePosition.Y < CameraScrollBoundY)
		MovementY = CameraSpeed * (1.0f - MousePosition.Y / CameraScrollBoundY);
	else if (MousePosition.Y > (ViewportSize.Y - CameraScrollBoundY))
		MovementY = -CameraSpeed * ((MousePosition.Y - (ViewportSize.Y - CameraScrollBoundY)) / CameraScrollBoundY);

	CameraRootComponent->AddRelativeLocation(FVector(MovementY, MovementX, 0.0f));
}

void URGPlayerCameraComponent::HandleMouseWheelInput(float Amount)
{
	if (!SpringArmComponent)
	{
		UE_LOG(LogRGPlayerCameraComponent, Warning, TEXT("[HandleMouseWheelInput] SpringArmComponent is nullptr."));
		return;
	}

	CameraZoomFactor = FMath::Clamp(CameraZoomFactor += Amount, 0.0f, 10.0f);
	SpringArmComponent->TargetArmLength = MAX_SPRING_ARM_LENGTH - (MAX_SPRING_ARM_LENGTH - MIN_SPRING_ARM_LENGTH) / 10.0f * CameraZoomFactor;
}
