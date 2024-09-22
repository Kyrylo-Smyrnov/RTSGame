// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/RGBuildingBase.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGBuildingBase, All, All);

ARGBuildingBase::ARGBuildingBase()
	: RemainingSpawnTime(0.0f), bIsSpawning(false), bIsSelected(false), bIsPlacing(false), bIsConstructing(false), TimeToConstruct(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponentCurrent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StaticMeshComponentCurrent->SetGenerateOverlapEvents(true);
	StaticMeshComponentCurrent->bReceivesDecals = false;
	SetRootComponent(StaticMeshComponentCurrent);

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
	if (bIsPlacing)
		HandleBuildingPlacing();
}

void ARGBuildingBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerController)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[HandleOnClicked] PlayerController is nullptr."));
		return;
	}

	if (bIsPlacing && CheckForOverlap())
	{
		StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

		bIsPlacing = false;
		SetBuildingMeshMaterials();
		HandleBuildingConstructing();
	}
	else if (ButtonPressed == EKeys::LeftMouseButton && !bIsPlacing)
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
	OnSpawnQueueChanged.Broadcast(SpawnQueue);

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

TArray<FSpawnQueueEntry>& ARGBuildingBase::GetSpawnQueue()
{
	return SpawnQueue;
}

bool ARGBuildingBase::GetIsConstructing() const
{
	return bIsConstructing;
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

void ARGBuildingBase::SetTimeToConstruct(float Time)
{
	TimeToConstruct = Time;
}

void ARGBuildingBase::SetBuildingPlacementMaterial(const bool IsValidPlacement)
{
	if (!StaticMeshComponentCurrent)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingPlacementMaterial] StaticMeshComponent is nullptr."));
		return;
	}
	if (!ValidPlacementMaterial || !InValidPlacementMaterial)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingPlacementMaterial] Placement materials aren't set."));
		return;
	}

	bIsPlacing = true;
	UMaterialInterface* PlacementMaterial = IsValidPlacement ? ValidPlacementMaterial : InValidPlacementMaterial;

	for (int i = 0; i < StaticMeshComponentCurrent->GetMaterials().Num(); ++i)
		StaticMeshComponentCurrent->SetMaterial(i, PlacementMaterial);
}

void ARGBuildingBase::SetBuildingMeshMaterials()
{
	if (!StaticMeshComponentCurrent)
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[SetBuildingMeshMaterials] StaticMeshComponent is nullptr."));

	for (int i = 0; i < BuildingMeshMaterials.Num(); ++i)
		StaticMeshComponentCurrent->SetMaterial(i, BuildingMeshMaterials[i]);
}

void ARGBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ARGBuildingBase::HandleOnClicked);
	BuildingMeshMaterials = StaticMeshComponentCurrent->GetMaterials();

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

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, [this, CurrentEntry]() {
		if (SpawnQueue.Num() > 0)
		{
			RemainingSpawnTime -= 0.1;
			float Progress = 1.0f - (RemainingSpawnTime / CurrentEntry.SpawnTime);
			OnSpawnProgressChanged.Broadcast(Progress);

			if (RemainingSpawnTime <= 0)
			{
				FVector SpawnLocation = GetActorLocation() + FVector(500, 0, 0);
				SpawnLocation.Z = 108;
				GetWorld()->SpawnActor<AActor>(CurrentEntry.UnitClass, SpawnLocation, FRotator::ZeroRotator);

				SpawnQueue.RemoveAt(0);
				OnSpawnQueueChanged.Broadcast(SpawnQueue);

				SpawnNextUnit();
			}
		} }, 0.1, true);
}

void ARGBuildingBase::HandleBuildingPlacing()
{
	FHitResult HitResult;
	if (PlayerController && PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
	{
		StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

		SetActorLocation(HitResult.Location);
		SetBuildingPlacementMaterial(CheckForOverlap());
	}
}

void ARGBuildingBase::HandleBuildingConstructing()
{
	if (TimeToConstruct == 0.0f || !StaticMeshConstructionPhase1 ||
		!StaticMeshConstructionPhase2 || !StaticMeshConstructionPhase3)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[HandleBuildingConstructing] Invalid input to perform constructing."));
		return;
	}

	bIsConstructing = true;
	StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase1);

	float TimeToChangePhase = TimeToConstruct / 2.0f;
	FTimerHandle Phase2TimerHandle;

	// TODO: TurnOff ActionButtons during construction.
	// TODO: Show construction progress in SelectionBarQueue.
	GetWorld()->GetTimerManager().SetTimer(Phase2TimerHandle, [this, TimeToChangePhase] {
		StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase2);
		FTimerHandle Phase3TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(Phase3TimerHandle,[this, TimeToChangePhase] {
			StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase3);
			bIsConstructing = false;
		}, TimeToChangePhase, false); }, TimeToChangePhase, false);
}

bool ARGBuildingBase::CheckForOverlap()
{
	TArray<AActor*> OverlappingActors;
	StaticMeshComponentCurrent->GetOverlappingActors(OverlappingActors);

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
