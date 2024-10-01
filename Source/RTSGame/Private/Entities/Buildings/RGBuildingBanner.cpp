// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingBanner.h"

ARGBuildingBanner::ARGBuildingBanner()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->bReceivesDecals = false;
	SetRootComponent(SkeletalMeshComponent);
}

void ARGBuildingBanner::BeginPlay()
{
	Super::BeginPlay();
}
