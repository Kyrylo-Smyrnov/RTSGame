// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/RGUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Entities/ActionsTooltips.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerPawn.h"
#include "RGPlayerController.h"

ARGUnitBase::ARGUnitBase()
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
		ARGPlayerPawn* PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
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

bool ARGUnitBase::IsSelected() const
{
	return bIsSelected;
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
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

	FActionData AttackAction;
	AttackAction.ActionName = "AttackAction";
	AttackAction.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconAttack"));
	AttackAction.ActionTooltip = Tooltips::Units::AttackActionTooltip;
	BaseUnitActions.Add(AttackAction);

	FActionData HoldAction;
	HoldAction.ActionName = "HoldAction";
	HoldAction.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconHold"));
	HoldAction.ActionTooltip = Tooltips::Units::HoldActionTooltip;
	BaseUnitActions.Add(HoldAction);

	FActionData MoveAction;
	MoveAction.ActionName = "MoveAction";
	MoveAction.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMove"));
	MoveAction.ActionTooltip = Tooltips::Units::MoveActionTooltip;
	BaseUnitActions.Add(MoveAction);

	FActionData MoveAttackAction;
	MoveAttackAction.ActionName = "MoveAttackAction";
	MoveAttackAction.ActionIcon =
		LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMoveAttack"));
	MoveAttackAction.ActionTooltip = Tooltips::Units::MoveAttackActionTooltip;
	BaseUnitActions.Add(MoveAttackAction);

	return BaseUnitActions;
}

void ARGUnitBase::PerformAction_Implementation(const FName& ActionName)
{
	IActionable::PerformAction_Implementation(ActionName);
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGUnitBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
