// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Player/RGPlayerCameraComponent.h"
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

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ARGPlayerPawn::HandleOnClicked);
}

void ARGPlayerPawn::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(TouchedActor);

	if(CastedUnit && ButtonPressed == EKeys::LeftMouseButton)
	{
		const bool bIsSelected = SelectedEntities.Contains(CastedUnit);
		CastedUnit->SetSelected(!bIsSelected);
		if(!bIsSelected)
			SelectedEntities.AddUnique(CastedUnit);
	}
}
