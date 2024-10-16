// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Entities/Actions/RGMoveToAction.h"
#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitBase.h"
#include "Player/RGPlayerCameraComponent.h"
#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGPlayerPawn, All, All);

ARGPlayerPawn::ARGPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<URGPlayerCameraComponent>("PlayerCameraComponent");
	SetRootComponent(PlayerCameraComponent);

	PlayerWoodResource = 0;
}

void ARGPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARGPlayerPawn::HandleLeftMouseButtonInputPressedUninteractable()
{
	if (bIsAwaitingTarget)
	{
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
		{
			ExecuteActionWithTarget(HitResult.Location, PlayerController->IsInputKeyDown(EKeys::LeftShift));
		}
	}
	else
	{
		ClearSelectedEntities();
	}
}

AActor* ARGPlayerPawn::GetMostImportantEntity() const
{
	if (SelectedEntities.Num() > 0)
		return SelectedEntities[0];

	return nullptr;
}

TArray<AActor*> ARGPlayerPawn::GetSelectedEntities() const
{
	return SelectedEntities;
}

void ARGPlayerPawn::AddEntitiesToSelected(AActor* Entity)
{
	if (Entity == nullptr)
	{
		UE_LOG(LogRGPlayerPawn, Warning, TEXT("[AddEntitiesToSelected] Entity is nullptr."));
		return;
	}

	SelectedEntities.AddUnique(Entity);

	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
		CastedUnit->SetSelected(true);
	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
		CastedBuilding->SetSelected(true);

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnMostImportantEntityChanged.Broadcast(GetMostImportantEntity());
	OnSelectedEntitiesChanged.Broadcast(SelectedEntities);
	if (AwaitingAction)
	{
		bIsAwaitingTarget = false;
		AwaitingAction = nullptr;
	}
}

void ARGPlayerPawn::AddEntitiesToSelected(TArray<AActor*> Entities)
{
	if (Entities.Num() == 0)
		return;

	for (AActor* Entity : Entities)
	{
		if (Entity == nullptr)
		{
			UE_LOG(LogRGPlayerPawn, Warning, TEXT("[AddEntitiesToSelected] Entity is nullptr."));
			continue;
		}

		SelectedEntities.AddUnique(Entity);

		if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
			CastedUnit->SetSelected(true);
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
			CastedBuilding->SetSelected(true);
	}

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnMostImportantEntityChanged.Broadcast(GetMostImportantEntity());
	OnSelectedEntitiesChanged.Broadcast(SelectedEntities);
	if (AwaitingAction)
	{
		bIsAwaitingTarget = false;
		AwaitingAction = nullptr;
	}
}

void ARGPlayerPawn::RemoveEntityFromSelected(AActor* Entity)
{
	if (Entity == nullptr)
	{
		UE_LOG(LogRGPlayerPawn, Warning, TEXT("[RemoveEntityFromSelected] Entity is nullptr."));
		return;
	}

	SelectedEntities.Remove(Entity);

	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
		CastedUnit->SetSelected(false);
	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
		CastedBuilding->SetSelected(false);

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnMostImportantEntityChanged.Broadcast(GetMostImportantEntity());
	OnSelectedEntitiesChanged.Broadcast(SelectedEntities);
	if (AwaitingAction)
	{
		bIsAwaitingTarget = false;
		AwaitingAction = nullptr;
	}
}

void ARGPlayerPawn::ClearSelectedEntities()
{
	if (SelectedEntities.Num() == 0)
		return;

	for (AActor* Entity : SelectedEntities)
	{
		if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
			CastedUnit->SetSelected(false);
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
			CastedBuilding->SetSelected(false);
	}

	SelectedEntities.Empty();

	OnMostImportantEntityChanged.Broadcast(GetMostImportantEntity());
	OnSelectedEntitiesChanged.Broadcast(SelectedEntities);
	if (AwaitingAction)
	{
		bIsAwaitingTarget = false;
		AwaitingAction = nullptr;
	}
}

bool ARGPlayerPawn::IsEntitySelected(AActor* Entity) const
{
	return SelectedEntities.Contains(Entity);
}

void ARGPlayerPawn::AddPlayerResources(int32 Amount)
{
	PlayerWoodResource += Amount;
	OnPlayerResourcesChanged.Broadcast(PlayerWoodResource);
}

int32 ARGPlayerPawn::GetPlayerResources() const
{
	return PlayerWoodResource;
}

void ARGPlayerPawn::SetAwaitingAction(IRGAction* Action)
{
	bIsAwaitingTarget = true;
	AwaitingAction = Action;
}

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARGPlayerController>(GetController());

	if (PlayerController)
		PlayerController->LeftMouseButtonInputPressedUninteractable.AddUObject(this, &ARGPlayerPawn::HandleLeftMouseButtonInputPressedUninteractable);
	else
		UE_LOG(LogRGPlayerPawn, Warning, TEXT("[BeginPlay] PlayerController is nullptr."))
}

bool ARGPlayerPawn::CompareEntityImportance(const AActor& A, const AActor& B)
{
	const auto* UnitA = Cast<ARGUnitBase>(&A);
	const auto* UnitB = Cast<ARGUnitBase>(&B);
	const auto* BuildingA = Cast<ARGBuildingBase>(&A);
	const auto* BuildingB = Cast<ARGBuildingBase>(&B);

	if (UnitA && UnitB)
		return UnitA->GetImportance() < UnitB->GetImportance();
	if (BuildingA && BuildingB)
		return BuildingA->GetImportance() < BuildingB->GetImportance();
	if (UnitA && BuildingB)
		return UnitA->GetImportance() < BuildingB->GetImportance();
	if (BuildingA && UnitB)
		return BuildingA->GetImportance() < UnitB->GetImportance();

	return false;
}

void ARGPlayerPawn::ExecuteActionWithTarget(FVector TargetLocation, bool bMustBeEnqueued)
{
	if (AwaitingAction)
	{
		TArray<ARGUnitBase*> MustBePerformedBy;
		for (AActor* Entity : SelectedEntities)
		{
			if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
			{
				if (CastedUnit->CanPerformAction(AwaitingAction))
				{
					MustBePerformedBy.Add(CastedUnit);
				}
			}
		}

		for(ARGUnitBase* Unit : MustBePerformedBy)
		{
			if(UObject* ActionObj = Cast<UObject>(AwaitingAction))
			{
				IRGAction* NewActionInstance = Cast<IRGAction>(DuplicateObject(ActionObj, this));

				if (URGMoveToAction* MoveToAction = Cast<URGMoveToAction>(NewActionInstance))
					MoveToAction->SetDestination(TargetLocation);

				if (IRGUnitAction* UnitAction = Cast<IRGUnitAction>(NewActionInstance))
					UnitAction->InitializeAction(Unit);

				if (bMustBeEnqueued)
				{
					Unit->AddActionToQueue(NewActionInstance);
				}
				else
				{
					Unit->ClearActionQueue();
					Unit->AddActionToQueue(NewActionInstance);
				}
			}
		}

		bIsAwaitingTarget = false;
		AwaitingAction = nullptr;
	}
}