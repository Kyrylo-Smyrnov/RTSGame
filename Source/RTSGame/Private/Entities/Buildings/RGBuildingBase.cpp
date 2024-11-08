// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingBase.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
#include "Entities/Buildings/RGBuildingBanner.h"
#include "Entities/Units/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"
#include "Player/UI/RGActionGridWidget.h"
#include "Player/UI/RGPlayerHUD.h"

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

bool ARGBuildingBase::GetIsSelected() const
{
	return bIsSelected;
}

bool ARGBuildingBase::GetIsConstructing() const
{
	return bIsConstructing;
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

TArray<UBaseAction*> ARGBuildingBase::GetAvailableActions()
{
	return AvailableActions;
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

	if (bIsBuildingSelected && PlayerPawn->GetMostImportantEntity() == this)
	{
		if (!BuildingBanner)
		{
			BuildingBanner = GetWorld()->SpawnActor<ARGBuildingBanner>(BuildingBannerClass, LastBannerLocation, FRotator::ZeroRotator);
		}
	}
	else
	{
		BuildingBanner->Destroy();
		BuildingBanner = nullptr;
	}
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

void ARGBuildingBase::SetBannerLocation(FVector NewLocation)
{
	if (BuildingBanner)
	{
		BuildingBanner->Destroy();
		BuildingBanner = nullptr;
	}

	BuildingBanner = GetWorld()->SpawnActor<ARGBuildingBanner>(BuildingBannerClass, NewLocation, FRotator::ZeroRotator);
	LastBannerLocation = NewLocation;
}

void ARGBuildingBase::SetActorToAttackForSpawnedUnit(AActor* NewActorToAttack)
{
	ActorToAttackForSpawnedUnits = NewActorToAttack;
}

void ARGBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ARGBuildingBase::HandleOnClicked);
	BuildingMeshMaterials = StaticMeshComponentCurrent->GetMaterials();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));

	PlayerController->RightMouseButtonInputPressed.AddUObject(this, &ARGBuildingBase::HandleRightMouseButtonInputPressed);

	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[BeginPlay] PlayerPawn is nullptr."));
		return;
	}

	if (!bIsPlacing)
		LastBannerLocation = GetActorLocation() + FVector(0.0f, 500.0f, 0.0f);

	InitializeActions();
}

void ARGBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsPlacing)
		HandleBuildingPlacing();
}

void ARGBuildingBase::InitializeActions() { }

void ARGBuildingBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerController || !PlayerPawn)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[HandleOnClicked] PlayerController or PlayerPawn is nullptr."));
		return;
	}

	if (bIsPlacing)
	{
		if (CheckForOverlap())
		{
			StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			StaticMeshComponentCurrent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

			bIsPlacing = false;
			LastBannerLocation = GetActorLocation() + FVector(0.0f, 500.0f, 0.0f);
			SetBuildingMeshMaterials();
			HandleBuildingConstructing();
		}
	}
	else if (ButtonPressed == EKeys::LeftMouseButton)
	{
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

bool ARGBuildingBase::CheckForOverlap() const
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
				FVector SpawnLocation = GetActorLocation() + FVector(0, 500, 0);
				SpawnLocation.Z = 108;
				ARGUnitBase* SpawnedUnit = GetWorld()->SpawnActor<ARGUnitBase>(CurrentEntry.UnitClass, SpawnLocation, FRotator::ZeroRotator);

				UMoveToAction* MoveToAction = NewObject<UMoveToAction>(this);
				MoveToAction->InitializeAction(SpawnedUnit);
				MoveToAction->SetDestination(LastBannerLocation);
				MoveToAction->Execute_Implementation();

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
	float ConstructionStartTime = GetWorld()->GetTimeSeconds();

	StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase1);

	float TimeToChangePhase = TimeToConstruct / 2.0f;

	FTimerHandle ConstructionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ConstructionTimerHandle, [this, TimeToChangePhase, &ConstructionTimerHandle, ConstructionStartTime]() {
		float ElapsedTime = GetWorld()->GetTimeSeconds() - ConstructionStartTime;
		float ConstructionProgress = ElapsedTime / TimeToConstruct;
		
		OnConstructionProgressChanged.Broadcast(ConstructionProgress);
		
		if (ElapsedTime >= TimeToChangePhase && ElapsedTime < TimeToConstruct)
		{
			StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase2);
		}
		else if (ElapsedTime >= TimeToConstruct)
		{
			StaticMeshComponentCurrent->SetStaticMesh(StaticMeshConstructionPhase3);
			
			GetWorld()->GetTimerManager().ClearTimer(ConstructionTimerHandle);
			bIsConstructing = false;
			
			OnConstructionProgressChanged.Broadcast(0.0f);
			OnSpawnQueueChanged.Broadcast(SpawnQueue);

			if(Cast<ARGBuildingBase>(PlayerPawn->GetMostImportantEntity()) == this)
			{
				if(ARGPlayerHUD* PlayerHUD = Cast<ARGPlayerHUD>(PlayerController->GetHUD()))
				{
					PlayerHUD->GetActionGridWidget()->UpdateWidget(PlayerPawn->GetMostImportantEntity());
				}
			}
		} }, 0.1f, true);
}

void ARGBuildingBase::HandleRightMouseButtonInputPressed()
{
	if (PlayerPawn->GetMostImportantEntity() != this)
		return;

	if (!BuildingBannerClass)
	{
		UE_LOG(LogRGBuildingBase, Warning, TEXT("[HandleRightMouseButtonInputPressedUninteractable] BuildingBannerClass is nullptr."));
		return;
	}

	FHitResult HitResult;
	if (!PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult) &&
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
	{
		SetBannerLocation(HitResult.Location);
		SetActorToAttackForSpawnedUnit(nullptr);
	}
}
