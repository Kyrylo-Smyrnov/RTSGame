// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/Components/EntityHandlerComponent.h"

#include "Entities/Buildings/RGBuildingBase.h"
#include "Entities/Units/RGUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"

UEntityHandlerComponent::UEntityHandlerComponent() : PlayerController(nullptr), MostImportantEntity(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEntityHandlerComponent::IsEntitySelected(AActor* Entity) const
{
	if (!Entity)
	{
		return false;
	}

	if (ARGUnitBase* Unit = Cast<ARGUnitBase>(Entity))
	{
		return SelectedUnits.Contains(Unit);
	}
	else if (ARGBuildingBase* Building = Cast<ARGBuildingBase>(Entity))
	{
		return SelectedBuildings.Contains(Building);
	}

	return false;
}

void UEntityHandlerComponent::AddUnitToSelected(ARGUnitBase* Unit)
{
	if (Unit)
	{
		SelectedUnits.AddUnique(Unit);
		IdentifyMostImportantEntity();

		Unit->SetSelected(true);

		if (OnSelectedEntitiesChanged.IsBound())
		{
			OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
		}
	}
}

void UEntityHandlerComponent::AddBuildingToSelected(ARGBuildingBase* Building)
{
	if (Building)
	{
		SelectedBuildings.AddUnique(Building);
		IdentifyMostImportantEntity();

		Building->SetSelected(true);

		if (OnSelectedEntitiesChanged.IsBound())
		{
			OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
		}
	}
}

void UEntityHandlerComponent::AddEntitiesToSelected(TArray<AActor*> Entities)
{
	if (Entities.Num() == 0)
	{
		ClearSelectedEntities();
		return;
	}

	if (SelectedUnits.Num() > 0 || SelectedBuildings.Num() > 0)
	{
		ClearSelectedEntities();
	}

	for (AActor* Entity : Entities)
	{
		if (Entity)
		{
			if (ARGUnitBase* Unit = Cast<ARGUnitBase>(Entity))
			{
				AddUnitToSelected(Unit);
			}
			else if (ARGBuildingBase* Building = Cast<ARGBuildingBase>(Entity))
			{
				AddBuildingToSelected(Building);
			}
		}
	}

	if (OnSelectedEntitiesChanged.IsBound())
	{
		OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
	}
}

void UEntityHandlerComponent::RemoveUnitFromSelected(ARGUnitBase* Unit)
{
	if (Unit)
	{
		SelectedUnits.Remove(Unit);
		Unit->SetSelected(false);

		if (SelectedUnits.Num() > 0 || SelectedBuildings.Num() > 0)
		{
			IdentifyMostImportantEntity();
		}

		if (OnSelectedEntitiesChanged.IsBound())
		{
			OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
		}
	}
}

void UEntityHandlerComponent::RemoveBuildingFromSelected(ARGBuildingBase* Building)
{
	if (Building)
	{
		SelectedBuildings.Remove(Building);
		Building->SetSelected(false);

		if (SelectedBuildings.Num() > 0 || SelectedUnits.Num() > 0)
		{
			IdentifyMostImportantEntity();
		}

		if (OnSelectedEntitiesChanged.IsBound())
		{
			OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
		}
	}
}

void UEntityHandlerComponent::ClearSelectedEntities()
{
	for (ARGUnitBase* Unit : SelectedUnits)
	{
		Unit->SetSelected(false);
	}
	SelectedUnits.Empty();

	for (ARGBuildingBase* Building : SelectedBuildings)
	{
		Building->SetSelected(false);
	}
	SelectedBuildings.Empty();

	MostImportantEntity = nullptr;

	if (OnMostImportantEntityChanged.IsBound())
	{
		OnMostImportantEntityChanged.Broadcast(MostImportantEntity);
	}

	if (OnSelectedEntitiesChanged.IsBound())
	{
		OnSelectedEntitiesChanged.Broadcast(GetSelectedEntities());
	}
}

AActor* UEntityHandlerComponent::GetMostImportantEntity() const
{
	if (MostImportantEntity)
	{
		return MostImportantEntity;
	}

	return nullptr;
}

TArray<AActor*> UEntityHandlerComponent::GetSelectedEntities() const
{
	TArray<AActor*> SelectedEntities;

	if (!SelectedUnits.Num() == 0)
	{
		for (ARGUnitBase* Unit : SelectedUnits)
		{
			SelectedEntities.AddUnique(Cast<AActor>(Unit));
		}
	}

	if (!SelectedBuildings.Num() == 0)
	{
		for (ARGBuildingBase* Building : SelectedBuildings)
		{
			SelectedEntities.AddUnique(Cast<AActor>(Building));
		}
	}

	if (SelectedEntities.Num() > 1)
	{
		SelectedEntities.Sort(CompareEntityImportance);
	}

	return SelectedEntities;
}

void UEntityHandlerComponent::HandleLeftMouseButtonInputPressed()
{
	FHitResult HitResult;

	if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
	{
	}
	else if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
	{
		ClearSelectedEntities();
	}
}

void UEntityHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->LeftMouseButtonInputPressed.AddUObject(this, &UEntityHandlerComponent::HandleLeftMouseButtonInputPressed);
	}
}

void UEntityHandlerComponent::IdentifyMostImportantEntity()
{
	if (SelectedUnits.Num() > 0 && SelectedBuildings.Num() > 0)
	{
		SelectedUnits.Sort(CompareEntityImportance);
		SelectedBuildings.Sort(CompareEntityImportance);

		AActor* MostImportantUnit = Cast<AActor>(SelectedUnits[0]);
		AActor* MostImportantBuilding = Cast<AActor>(SelectedBuildings[0]);

		CompareEntityImportance(*MostImportantUnit, *MostImportantBuilding) ? MostImportantEntity = MostImportantUnit : MostImportantEntity = MostImportantBuilding;
	}
	else if (SelectedUnits.Num() > 0)
	{
		SelectedUnits.Sort(CompareEntityImportance);
		MostImportantEntity = Cast<AActor>(SelectedUnits[0]);
	}
	else
	{
		SelectedBuildings.Sort(CompareEntityImportance);
		MostImportantEntity = Cast<AActor>(SelectedBuildings[0]);
	}

	if (OnMostImportantEntityChanged.IsBound())
	{
		OnMostImportantEntityChanged.Broadcast(MostImportantEntity);
	}
}

bool UEntityHandlerComponent::CompareEntityImportance(const AActor& A, const AActor& B)
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
