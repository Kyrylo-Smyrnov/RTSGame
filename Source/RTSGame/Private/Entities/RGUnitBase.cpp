// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/RGUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Entities/Actions.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogUnitBase, All, All);

ARGUnitBase::ARGUnitBase() : bIsSelected(false)
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bReceivesDecals = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
}

void ARGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGUnitBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (PlayerController && ButtonPressed == EKeys::LeftMouseButton)
	{
		if (!PlayerPawn)
		{
			UE_LOG(LogUnitBase, Warning, TEXT("[HandleOnClicked] PlayerPawn is nullptr."))
			return;
		}

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

bool ARGUnitBase::IsSelected() const
{
	return bIsSelected;
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
	if(!SelectionCircleDecal)
	{
		UE_LOG(LogUnitBase, Warning, TEXT("[SetSelected] SelectionCircleDecal is nullptr."));
		return;
	}
	
	bIsSelected = bIsUnitSelected;
	SelectionCircleDecal->SetVisibility(bIsUnitSelected);
}

int32 ARGUnitBase::GetImportance() const
{
	return UnitImportance;
}

TArray<FActionData> ARGUnitBase::GetAvailableActions_Implementation() const
{
	TArray<FActionData> BaseUnitActions;

	BaseUnitActions.Add(UnitActions::Base_Attack);
	BaseUnitActions.Add(UnitActions::Base_Hold);
	BaseUnitActions.Add(UnitActions::Base_Move);
	BaseUnitActions.Add(UnitActions::Base_MoveAttack);

	return BaseUnitActions;
}

void ARGUnitBase::PerformAction_Implementation(const FName& ActionName)
{
	if(ActionName == "AttackAction")
	{
		UE_LOG(LogUnitBase, Warning, TEXT("AttackAction logic is not implemented yet."));
		return;
	}
	else if(ActionName == "HoldAction")
	{
		UE_LOG(LogUnitBase, Warning, TEXT("HoldAction logic is not implemented yet."));
		return;
	}
	else if(ActionName == "MoveAction")
	{
		UE_LOG(LogUnitBase, Warning, TEXT("MoveAction logic is not implemented yet."));
		return;
	}
	else if(ActionName == "MoveAttackAction")
	{
		UE_LOG(LogUnitBase, Warning, TEXT("MoveAttackAction logic is not implemented yet."));
		return;
	}
	
	IActionable::PerformAction_Implementation(ActionName);
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGUnitBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(!PlayerController)
	{
		UE_LOG(LogUnitBase, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));
		return;
	}
	
	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if(!PlayerPawn)
	{
		UE_LOG(LogUnitBase, Warning, TEXT("[BeginPlay] PlayerPawn is nullptr."));
		return;
	}
}
