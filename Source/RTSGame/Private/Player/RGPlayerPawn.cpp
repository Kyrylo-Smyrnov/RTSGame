// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Player/RGPlayerCameraComponent.h"
#include "RGPlayerController.h"
#include "Units/RGUnitBase.h"

ARGPlayerPawn::ARGPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCameraComponent = CreateDefaultSubobject<URGPlayerCameraComponent>("PlayerCameraComponent");

	SetRootComponent(PlayerCameraComponent);
}

void ARGPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARGPlayerPawn::HandleLeftMouseButtonInputPressedUninteractable()
{
	ClearSelectedEntities();
}

void ARGPlayerPawn::AddEntityToSelected(AActor* Entity)
{
	SelectedEntities.AddUnique(Entity);
	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
	{
		CastedUnit->SetSelected(true);
	}
}

void ARGPlayerPawn::RemoveEntityFromSelected(AActor* Entity)
{
	SelectedEntities.Remove(Entity);
	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
	{
		CastedUnit->SetSelected(false);
	}
}

void ARGPlayerPawn::ClearSelectedEntities()
{
	for (int32 i = SelectedEntities.Num() - 1; i >= 0; --i)
		RemoveEntityFromSelected(SelectedEntities[i]);
}

bool ARGPlayerPawn::IsEntitySelected(AActor* Entity) const
{
	return SelectedEntities.Contains(Entity);
}

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARGPlayerController>(GetController());

	if (PlayerController)
		PlayerController->LeftMouseButtonInputPressedUninteractable.AddUObject(
			this, &ARGPlayerPawn::HandleLeftMouseButtonInputPressedUninteractable);
}