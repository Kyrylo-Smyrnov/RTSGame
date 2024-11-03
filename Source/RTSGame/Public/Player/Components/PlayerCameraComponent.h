// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ARGPlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RTSGAME_API UPlayerCameraComponent : public USceneComponent
{
	GENERATED_BODY()

  public:
	UPlayerCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* CameraRootComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float CameraSpeed = 50.0f;

  private:
	void HandleCameraMovement();
	void HandleMouseWheelInput(float Amount);

	UPROPERTY()
	ARGPlayerController* PlayerController;

	FIntPoint ViewportSize = FIntPoint::ZeroValue;
	
	int CameraScrollBoundX = 0.0f;
	int CameraScrollBoundY = 0.0f;
	const float SCROLL_BOUNDARY_FRACTION = 0.1f;

	float CameraZoomFactor = 0.0f;
	const float MAX_SPRING_ARM_LENGTH = 2000.0f;
	const float MIN_SPRING_ARM_LENGTH = 500.0f;
};
