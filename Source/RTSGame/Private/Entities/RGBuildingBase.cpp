// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/RGBuildingBase.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

ARGBuildingBase::ARGBuildingBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponent);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
}

void ARGBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGBuildingBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (PlayerController && ButtonPressed == EKeys::LeftMouseButton)
	{
		ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
		if (!PlayerPawn)
			return;

		bool bIsShiftDown = PlayerController->IsInputKeyDown(EKeys::LeftShift);
		bool bIsCtrlDown = PlayerController->IsInputKeyDown(EKeys::LeftControl);

		if (!PlayerPawn->IsEntitySelected(this))
		{
			if (bIsShiftDown)
			{
				// Shift + Click: add a entity to selected
				PlayerPawn->AddEntityToSelected(this);
			}
			else
			{
				// Click without Shift: deselect all other entities and select clicked entity.
				PlayerPawn->ClearSelectedEntities();
				PlayerPawn->AddEntityToSelected(this);
			}
		}
		else if (bIsCtrlDown)
		{
			// Ctrl + Click: Remove entity from selected
			PlayerPawn->RemoveEntityFromSelected(this);
		}
		else
		{
			// Click on an already selected entity without Shift and Ctrl
			// Deselect all other entities and leave this one selected
			PlayerPawn->ClearSelectedEntities();
			PlayerPawn->AddEntityToSelected(this);
		}
	}
}

bool ARGBuildingBase::IsSeleted() const
{
	return bIsSelected;
}

void ARGBuildingBase::SetSelected(bool bIsBuildingSelected)
{
	bIsSelected = bIsBuildingSelected;
	SelectionCircleDecal->SetVisibility(bIsBuildingSelected);
}

int32 ARGBuildingBase::GetImportance() const
{
	return BuildingImportance;
}

void ARGBuildingBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGBuildingBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
