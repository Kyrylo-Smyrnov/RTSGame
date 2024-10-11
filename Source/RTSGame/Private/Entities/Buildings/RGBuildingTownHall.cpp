// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"

#include "Entities/Actions/RGActionsList.h"
#include "Entities/Units/RGUnitPeasant.h"

DEFINE_LOG_CATEGORY_STATIC(LogBuildingTownHall, All, All);

ARGBuildingTownHall::ARGBuildingTownHall()
	: ARGBuildingBase()
{
	this->BuildingImportance = EFEntitiesImportance::TownHall;
}

void ARGBuildingTownHall::BeginPlay()
{
	Super::BeginPlay();
}

void ARGBuildingTownHall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARGBuildingTownHall::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Super::HandleOnClicked(TouchedActor, ButtonPressed);
	TArray<AActor*> SelectedEntities = PlayerPawn->GetSelectedEntities();
	for (int i = 0; i < SelectedEntities.Num(); ++i)
	{
		if (ARGUnitPeasant* CastedPeasant = Cast<ARGUnitPeasant>(SelectedEntities[i]))
		{
			if (CastedPeasant->GetIsCarryingResources())
			{
				UBlackboardComponent* Blackboard = Cast<ARGUnitAIController>(CastedPeasant->GetController())->GetBlackboardComponent();
				Blackboard->SetValueAsEnum(BBKeys::UNIT_AI_BBKEY_UNITSTATE, 2);
			}
		}
	}
}

void ARGBuildingTownHall::InitializeActions()
{
	Super::InitializeActions();
	
	URGSpawnUnitAction* SpawnPeasantAction = NewObject<URGSpawnUnitAction>();
	SpawnPeasantAction->InitializeAction(UnitPeasantBlueprintClass, this, PlayerPawn);
	FRGActionData BuildPeasantData = BuildingActions::TownHall_BuildPeasant;
	BuildPeasantData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Buildings/TownHall/T_IconBuildPeasant"));
	SpawnPeasantAction->SetActionData(BuildPeasantData);
	AvailableActions.Add(SpawnPeasantAction);
}
