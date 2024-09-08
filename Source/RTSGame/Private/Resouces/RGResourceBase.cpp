// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Resouces/RGResourceBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

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

void ARGResourceBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::RightMouseButton)
	{
		for (AActor* SelectedEntity : PlayerPawn->GetSelectedEntities())
		{
			ARGUnitBase* Unit = Cast<ARGUnitBase>(SelectedEntity);
			AAIController* AIController = Cast<AAIController>(Unit->GetController());
			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

			UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			FNavLocation TargetLocation;
			NavigationSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 50, TargetLocation);

			// 1 - Attack
			BlackboardComponent->SetValueAsEnum("UnitState", 1);
			BlackboardComponent->SetValueAsVector("TargetLocationToMove", TargetLocation.Location);
		}
	}
}

void ARGResourceBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGResourceBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
}