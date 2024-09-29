// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Resouces/RGResourceBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Units/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGResourceBase, All, All);

ARGResourceBase::ARGResourceBase()
	: bIsDead(false), Health(100.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMeshComponent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponent);
}

void ARGResourceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGResourceBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerPawn)
	{
		UE_LOG(LogRGResourceBase, Warning, TEXT("[HandleOnClicked] PlayerPawn is nullptr."))
		return;
	}

	if (ButtonPressed == EKeys::RightMouseButton)
	{
		for (AActor* SelectedEntity : PlayerPawn->GetSelectedEntities())
		{
			ARGUnitBase* Unit = Cast<ARGUnitBase>(SelectedEntity);
			if (Unit)
			{
				AAIController* AIController = Cast<AAIController>(Unit->GetController());
				if (!AIController)
				{
					UE_LOG(LogRGResourceBase, Warning, TEXT("[HandleOnClicked] AIController is nullptr."))
					return;
				}

				UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
				if (!BlackboardComponent)
				{
					UE_LOG(LogRGResourceBase, Warning, TEXT("[HandleOnClicked] BlackboardComponent is nullptr."))
					return;
				}

				// 1 - Attack
				BlackboardComponent->SetValueAsEnum(BLACKBOARD_KEY_UNITSTATE, 1);
				BlackboardComponent->SetValueAsVector(BLACKBOARD_KEY_TARGETLOCATIONTOMOVE, GetActorLocation());
				BlackboardComponent->SetValueAsObject(BLACKBOARD_KEY_TARGETACTORTOATTACK, this);
			}
		}
	}
}

void ARGResourceBase::DealDamage(const float Amount)
{
	Health -= Amount;

	if (Health <= 0)
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
	if (!PlayerController)
	{
		UE_LOG(LogRGResourceBase, Warning, TEXT("[BeginPlay] PlayerController is nullptr."))
		return;
	}

	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGResourceBase, Warning, TEXT("[BeginPlay] PlayerPawn is nullptr."))
		return;
	}
}