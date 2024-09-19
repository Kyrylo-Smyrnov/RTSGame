// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/RGBuildingBase.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGBuildingBase, All, All);

ARGBuildingBase::ARGBuildingBase()
	: bIsSelected(false), bIsConstructing(false), bIsSpawning(false), RemainingSpawnTime(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponent);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ValidPlacementMaterialFinder(
		TEXT("/Game/Entities/Buildings/Materials/M_BuildingValidPlacement.M_BuildingValidPlacement"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> InValidPlacementMaterialFinder(
		TEXT("/Game/Entities/Buildings/Materials/M_BuildingInvalidPlacement.M_BuildingInvalidPlacement"));

	if (ValidPlacementMaterialFinder.Succeeded())
		ValidPlacementMaterial = ValidPlacementMaterialFinder.Object;
	else
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[Constructor] ValidPlacementMaterial not found."));

	if (InValidPlacementMaterialFinder.Succeeded())
		InValidPlacementMaterial = InValidPlacementMaterialFinder.Object;
	else
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[Constructor] ValidPlacementMaterial not found."));
}

void ARGBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsConstructing)
		HandleBuildingConstructing();
}

void ARGBuildingBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerController)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[HandleOnClicked] PlayerController is nullptr."));
		return;
	}

	if (bIsConstructing && CheckForOverlap())
	{
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		bIsConstructing = false;
		SetBuildingMeshMaterials();
	}
	else if (ButtonPressed == EKeys::LeftMouseButton && !bIsConstructing)
	{
		if (!PlayerPawn)
			return;

		bool bIsShiftDown = PlayerController->IsInputKeyDown(EKeys::LeftShift);
		bool bIsCtrlDown = PlayerController->IsInputKeyDown(EKeys::LeftControl);

		if (!PlayerPawn->IsEntitySelected(this))
		{
			if (bIsShiftDown)
			{
				// Shift + Click: add a entity to selected
				PlayerPawn->AddEntitiesToSelected(this);
			}
			else
			{
				// Click without Shift: deselect all other entities and select clicked entity.
				PlayerPawn->ClearSelectedEntities();
				PlayerPawn->AddEntitiesToSelected(this);
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
			PlayerPawn->AddEntitiesToSelected(this);
		}
	}
}

void ARGBuildingBase::AddUnitToSpawnQueue(TSubclassOf<AActor> UnitClass, float SpawnTime)
{
	FSpawnQueueEntry QueueEntry;
	QueueEntry.UnitClass = UnitClass;
	QueueEntry.SpawnTime = SpawnTime;

	SpawnQueue.Add(QueueEntry);

	if (!bIsSpawning)
		SpawnNextUnit();
}

bool ARGBuildingBase::IsSelected() const
{
	return bIsSelected;
}

int32 ARGBuildingBase::GetImportance() const
{
	return BuildingImportance;
}

UTexture2D* ARGBuildingBase::GetSelectionIcon() const
{
	if (!SelectionIcon)
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[GetSelectionIcon] SelectionIcon is nullptr."))

	return SelectionIcon;
}

void ARGBuildingBase::SetSelected(bool bIsBuildingSelected)
{
	if (!SelectionCircleDecal)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetSelected] SelectionCircleDecal is nullptr."));
		return;
	}

	bIsSelected = bIsBuildingSelected;
	SelectionCircleDecal->SetVisibility(bIsBuildingSelected);
}

void ARGBuildingBase::SetBuildingPlacementMaterial(const bool IsValidPlacement)
{
	if (!StaticMeshComponent)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingPlacementMaterial] StaticMeshComponent is nullptr."));
		return;
	}
	if (!ValidPlacementMaterial || !InValidPlacementMaterial)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingPlacementMaterial] Placement materials aren't set."));
		return;
	}

	bIsConstructing = true;
	UMaterialInterface* PlacementMaterial = IsValidPlacement ? ValidPlacementMaterial : InValidPlacementMaterial;

	for (int i = 0; i < StaticMeshComponent->GetMaterials().Num(); ++i)
		StaticMeshComponent->SetMaterial(i, PlacementMaterial);
}

void ARGBuildingBase::SetBuildingMeshMaterials()
{
	if (!StaticMeshComponent)
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingMeshMaterials] StaticMeshComponent is nullptr."));

	for (int i = 0; i < BuildingMeshMaterials.Num(); ++i)
		StaticMeshComponent->SetMaterial(i, BuildingMeshMaterials[i]);
}

void ARGBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ARGBuildingBase::HandleOnClicked);
	BuildingMeshMaterials = StaticMeshComponent->GetMaterials();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));

	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[BeginPlay] PlayerPawn is nullptr."));
		return;
	}

	PlayerPawn->AddEntitiesToContolled(this);
}

void ARGBuildingBase::SpawnNextUnit()
{
	if (SpawnQueue.Num() == 0)
	{
		bIsSpawning = false;
		RemainingSpawnTime = 0.0f;
		return;
	}

	FSpawnQueueEntry CurrentEntry = SpawnQueue[0];

	bIsSpawning = true;
	RemainingSpawnTime = CurrentEntry.SpawnTime;

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this, CurrentEntry]() {
			FVector SpawnLocation = GetActorLocation() + FVector(500, 0, 0);
			SpawnLocation.Z = 108;
			GetWorld()->SpawnActor<AActor>(CurrentEntry.UnitClass, SpawnLocation, FRotator::ZeroRotator);

			SpawnQueue.RemoveAt(0);

			SpawnNextUnit();
		},
		CurrentEntry.SpawnTime,
		false);
}

void ARGBuildingBase::HandleBuildingConstructing()
{
	FHitResult HitResult;
	if (PlayerController && PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
	{
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

		SetActorLocation(HitResult.Location);
		SetBuildingPlacementMaterial(CheckForOverlap());
	}
}

bool ARGBuildingBase::CheckForOverlap()
{
	TArray<AActor*> OverlappingActors;
	StaticMeshComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetRootComponent()->GetCollisionObjectType() == ECC_Pawn ||
			Actor->GetRootComponent()->GetCollisionObjectType() == ECC_WorldDynamic)
		{
			return false;
		}
	}

	return true;
}
