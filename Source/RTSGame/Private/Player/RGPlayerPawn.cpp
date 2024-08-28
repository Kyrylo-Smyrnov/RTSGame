// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Player/RGPlayerPawn.h"
#include "Entities/RGBuildingBase.h"
#include "Entities/RGUnitBase.h"
#include "Player/RGPlayerCameraComponent.h"
#include "RGPlayerController.h"

ARGPlayerPawn::ARGPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCameraComponent = CreateDefaultSubobject<URGPlayerCameraComponent>("PlayerCameraComponent");

	SetRootComponent(PlayerCameraComponent);
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
	ClearSelectedEntities();
}

AActor* ARGPlayerPawn::GetMostImportantEntity() const
{
	if (SelectedEntities.Num() > 0)
		return SelectedEntities[0];
	return nullptr;
}

void ARGPlayerPawn::AddEntitiesToSelected(AActor* Entity)
{
	SelectedEntities.AddUnique(Entity);
	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
		CastedUnit->SetSelected(true);
	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
		CastedBuilding->SetSelected(true);

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnSelectedEntitiesChanged.Broadcast(GetMostImportantEntity());
}

void ARGPlayerPawn::AddEntitiesToSelected(TArray<AActor*> Entities)
{
	if(Entities.Num() == 0)
		return;
	
	for(AActor* Entity : Entities)
	{
		SelectedEntities.AddUnique(Entity);
		
		if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
			CastedUnit->SetSelected(true);
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
			CastedBuilding->SetSelected(true);
	}

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnSelectedEntitiesChanged.Broadcast(GetMostImportantEntity());
}

void ARGPlayerPawn::RemoveEntityFromSelected(AActor* Entity)
{
	SelectedEntities.Remove(Entity);
	if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
		CastedUnit->SetSelected(false);
	if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
		CastedBuilding->SetSelected(false);

	if (SelectedEntities.Num() > 1)
		SelectedEntities.Sort(CompareEntityImportance);

	OnSelectedEntitiesChanged.Broadcast(GetMostImportantEntity());
}

void ARGPlayerPawn::ClearSelectedEntities()
{
	if(SelectedEntities.Num() == 0)
		return;
	
	for (AActor* Entity : SelectedEntities)
	{
		if (ARGUnitBase* CastedUnit = Cast<ARGUnitBase>(Entity))
			CastedUnit->SetSelected(false);
		if (ARGBuildingBase* CastedBuilding = Cast<ARGBuildingBase>(Entity))
			CastedBuilding->SetSelected(false);
	}

	SelectedEntities.Empty();

	OnSelectedEntitiesChanged.Broadcast(GetMostImportantEntity());
}

bool ARGPlayerPawn::IsEntitySelected(AActor* Entity) const
{
	return SelectedEntities.Contains(Entity);
}

void ARGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARGPlayerController>(GetController());

	if (PlayerController)
		PlayerController->LeftMouseButtonInputPressedUninteractable.AddUObject(
			this, &ARGPlayerPawn::HandleLeftMouseButtonInputPressedUninteractable);
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