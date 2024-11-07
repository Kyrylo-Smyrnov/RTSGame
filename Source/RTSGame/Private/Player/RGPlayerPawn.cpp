// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Player/Components/EntityHandlerComponent.h"
#include "Player/Components/PlayerCameraComponent.h"

ARGPlayerPawn::ARGPlayerPawn() : PlayerResources(0)
{
	PrimaryActorTick.bCanEverTick = false;

	EntityHandler = CreateDefaultSubobject<UEntityHandlerComponent>("EntityHandler");
	CameraComponent = CreateDefaultSubobject<UPlayerCameraComponent>("CameraComponent");
	SetRootComponent(CameraComponent);
}

void ARGPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UEntityHandlerComponent* ARGPlayerPawn::GetEntityHandler() const
{
	return EntityHandler;
}

void ARGPlayerPawn::AddPlayerResources(int32 Amount)
{
	PlayerResources += Amount;
	OnPlayerResourcesChanged.Broadcast(PlayerResources);
}

int32 ARGPlayerPawn::GetPlayerResources() const
{
	return PlayerResources;
}

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}