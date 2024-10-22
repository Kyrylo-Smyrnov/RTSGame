// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
#include "Entities/Actions/Interfaces/TargetTypeActorAction.h"
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

void ARGPlayerPawn::HandleLeftMouseButtonInputPressed()
{
	FHitResult HitResult;
	
	if(PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
	{
		if(AwaitingAction && AwaitingAction->GetActionData().TargetType == EActionTargetType::Actor)
		{
			TVariant<FVector, AActor*> TargetVariant;
			if(HitResult.Actor.IsValid())
			{
				TargetVariant.Set<AActor*>(HitResult.Actor.Get());
				ExecuteActionWithTarget(TargetVariant, PlayerController->IsInputKeyDown(EKeys::LeftShift));
			}
		}
	}
	else if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
	{
		if (AwaitingAction && AwaitingAction->GetActionData().TargetType == EActionTargetType::Location)
		{
			TVariant<FVector, AActor*> TargetVariant;
			TargetVariant.Set<FVector>(HitResult.Location);
			ExecuteActionWithTarget(TargetVariant, PlayerController->IsInputKeyDown(EKeys::LeftShift));
			
		}
		else
		{
			ClearSelectedEntities();
		}
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

void ARGPlayerPawn::SetAwaitingAction(UBaseAction* Action)
{
	bIsAwaitingTarget = true;
	AwaitingAction = Action;
}

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARGPlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->LeftMouseButtonInputPressed.AddUObject(this, &ARGPlayerPawn::HandleLeftMouseButtonInputPressed);
	}
	else
	{
		UE_LOG(LogRGPlayerPawn, Warning, TEXT("[BeginPlay] PlayerController is nullptr."))
	}
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

void ARGPlayerPawn::ExecuteActionWithTarget(TVariant<FVector, AActor*> TargetVariant, bool bMustBeEnqueued)
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

	for (ARGUnitBase* Unit : MustBePerformedBy)
	{
		UBaseAction* NewActionInstance = DuplicateObject(AwaitingAction, this);
		if(NewActionInstance)
		{
			if(NewActionInstance->GetClass()->ImplementsInterface(UUnitAction::StaticClass()))
			{
				IUnitAction* UnitAction = Cast<IUnitAction>(NewActionInstance);
				if(UnitAction)
				{
					UnitAction->InitializeAction(Unit);
				}
			}
			if(TargetVariant.IsType<FVector>() && NewActionInstance->GetClass()->ImplementsInterface(UTargetTypeLocationAction::StaticClass()))
			{
				ITargetTypeLocationAction* TargetTypeLocationAction = Cast<ITargetTypeLocationAction>(NewActionInstance);
				if(TargetTypeLocationAction)
				{
					TargetTypeLocationAction->SetDestination(TargetVariant.Get<FVector>());
				}
			}
			if(TargetVariant.IsType<AActor*>() && NewActionInstance->GetClass()->ImplementsInterface(UTargetTypeActorAction::StaticClass()))
			{
				ITargetTypeActorAction* TargetTypeActorAction = Cast<ITargetTypeActorAction>(NewActionInstance);
				if(TargetTypeActorAction)
				{
					TargetTypeActorAction->SetTarget(TargetVariant.Get<AActor*>());
				}
			}
		}

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

	bIsAwaitingTarget = false;
	AwaitingAction = nullptr;
}