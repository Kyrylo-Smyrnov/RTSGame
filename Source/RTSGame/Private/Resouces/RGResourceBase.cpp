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

	Health = 100.0f;
	bIsDead = false;
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
			if(Unit)
			{
				AAIController* AIController = Cast<AAIController>(Unit->GetController());
				UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

				// 1 - Attack
				BlackboardComponent->SetValueAsEnum("UnitState", 1);
				BlackboardComponent->SetValueAsVector("TargetLocationToMove", GetActorLocation());
				BlackboardComponent->SetValueAsObject("TargetActorToAttack", this);
			}
		}
	}
}

void ARGResourceBase::DealDamage(const float Amount)
{
	Health -= Amount;
	
	if(Health <= 0)
	{
		bIsDead = true;
		Destroy();
	}
}

void ARGResourceBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGResourceBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
}