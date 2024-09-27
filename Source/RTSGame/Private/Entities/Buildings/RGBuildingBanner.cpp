// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingBanner.h"

ARGBuildingBanner::ARGBuildingBanner()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	SkeletalMeshComponent->bReceivesDecals = false;
	SetRootComponent(SkeletalMeshComponent);
}

void ARGBuildingBanner::BeginPlay()
{
	Super::BeginPlay();
}
