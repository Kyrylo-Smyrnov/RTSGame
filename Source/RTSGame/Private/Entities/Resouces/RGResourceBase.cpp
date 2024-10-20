// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Resouces/RGResourceBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/DecalComponent.h"
#include "Entities/BBKeys.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitBase.h"
#include "Entities/Units/RGUnitPeasant.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGResourceBase, All, All);

ARGResourceBase::ARGResourceBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMeshComponent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(GetRootComponent());
	DecalComponent->SetVisibility(false);

	ResourceAmount = 100;
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

	if (PlayerPawn->GetSelectedEntities().Num() == 0)
		return;

	if (ButtonPressed == EKeys::RightMouseButton)
	{
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(PlayerPawn->GetMostImportantEntity()))
		{
			CastedBuilding->SetBannerLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
			CastedBuilding->SetActorToAttackForSpawnedUnit(this);
		}
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
				BlackboardComponent->SetValueAsEnum(BBKeys::UNIT_AI_BBKEY_UNITSTATE, 1);
				BlackboardComponent->SetValueAsVector(BBKeys::UNIT_AI_BBKEY_TARGETLOCATIONTOMOVE, GetActorLocation());
				BlackboardComponent->SetValueAsObject(BBKeys::UNIT_AI_BBKEY_TARGETACTORTOATTACK, this);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, this, &ARGResourceBase::BlinkDecal, 0.25, true, 0.0f);
	}
}

void ARGResourceBase::ReceiveDamage(float DamageAmount, AActor* DamageCauser)
{
	ResourceAmount -= DamageAmount;
	ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(DamageCauser);
	if(CastedPeasant)
		CastedPeasant->AddCarryingWood(DamageAmount);
	
	UE_LOG(LogRGResourceBase, Warning, TEXT("[ReceiveDamage] TOCHANGE: Current Health: %i"), ResourceAmount);
	if(ResourceAmount <= 0)
	{
		Destroy();
		UE_LOG(LogRGResourceBase, Warning, TEXT("[ReceiveDamage] TOCHANGE: I'm Dead"));
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

void ARGResourceBase::BlinkDecal()
{
	bIsDecalVisible = !bIsDecalVisible;
	DecalComponent->SetVisibility(bIsDecalVisible);

	static int32 BlinkCount = 0;
	BlinkCount++;

	if (BlinkCount >= 4)
	{
		GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
		DecalComponent->SetVisibility(false);
		BlinkCount = 0;
	}
}