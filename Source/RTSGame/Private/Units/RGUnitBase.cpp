// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Units/RGUnitBase.h"
#include "Components/DecalComponent.h"

ARGUnitBase::ARGUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bReceivesDecals = false;

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
}

void ARGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
	bIsSelected = bIsUnitSelected;
	SelectionCircleDecal->SetVisibility(bIsUnitSelected);
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
}
