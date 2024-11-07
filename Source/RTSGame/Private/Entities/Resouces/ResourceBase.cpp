// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Resouces/ResourceBase.h"

#include "Entities/Actions/Implementation/CollectResourceAction.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitPeasant.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Components/EntityHandlerComponent.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"

AResourceBase::AResourceBase() : AAttackableBase(100)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMeshComponent->bReceivesDecals = false;
	StaticMeshComponent->SetupAttachment(Root);
}

void AResourceBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::RightMouseButton)
	{
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(EntityHandler->GetMostImportantEntity()))
		{
			CastedBuilding->SetBannerLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
			CastedBuilding->SetActorToAttackForSpawnedUnit(this);
		}

		for (AActor* SelectedEntity : EntityHandler->GetSelectedEntities())
		{
			ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(SelectedEntity);
			if (CastedUnit)
			{
				if (ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(CastedUnit))
				{
					UCollectResourceAction* CollectResourceAction = NewObject<UCollectResourceAction>();
					CollectResourceAction->InitializeAction(CastedPeasant);
					CollectResourceAction->SetTarget(this);

					if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftShift))
					{
						CastedPeasant->ClearActionQueue();
					}

					CastedPeasant->AddActionToQueue(CollectResourceAction);
				}
				else
				{
					UMoveToAction* MoveToAction = NewObject<UMoveToAction>();
					MoveToAction->InitializeAction(CastedUnit);
					MoveToAction->SetDestination(GetActorLocation());

					if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftShift))
					{
						CastedUnit->ClearActionQueue();
					}

					CastedUnit->AddActionToQueue(MoveToAction);
				}
			}
		}

		StartDecalBlinking();
	}
}

void AResourceBase::ReceiveDamage(float DamageAmount, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamageAmount, DamageCauser);
	
	ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(DamageCauser);
	if (CastedPeasant)
	{
		CastedPeasant->AddCarryingWood(DamageAmount);
	}
}

void AResourceBase::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &AResourceBase::HandleOnClicked);

	ARGPlayerController* PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		return;
	}

	EntityHandler = Cast<ARGPlayerPawn>(PlayerController->GetPawn())->GetEntityHandler();
}