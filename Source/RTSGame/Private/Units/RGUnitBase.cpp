// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Units/RGUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

ARGUnitBase::ARGUnitBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bReceivesDecals = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
}

void ARGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGUnitBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (PlayerController && ButtonPressed == EKeys::LeftMouseButton)
	{
		ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
		if (!PlayerPawn)
			return;

		if (!PlayerPawn->IsEntitySelected(this))
		{
			PlayerPawn->AddEntityToSelected(this);
		}
		else
		{
			PlayerPawn->RemoveEntityFromSelected(this);
		}
	}
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
	bIsSelected = bIsUnitSelected;
	SelectionCircleDecal->SetVisibility(bIsUnitSelected);
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGUnitBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
