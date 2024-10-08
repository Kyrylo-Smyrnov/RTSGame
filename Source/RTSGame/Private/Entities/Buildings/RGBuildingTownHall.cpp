// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Buildings/RGBuildingTownHall.h"
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
