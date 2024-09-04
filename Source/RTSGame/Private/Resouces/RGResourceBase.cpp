// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Resouces/RGResourceBase.h"

ARGResourceBase::ARGResourceBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponent);
}

void ARGResourceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGResourceBase::BeginPlay()
{
	Super::BeginPlay();
}