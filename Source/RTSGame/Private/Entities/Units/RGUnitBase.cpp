// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Entities/Actions/ActionQueue.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"
#include "Player/Components/EntityHandlerComponent.h"
#include "Player/RGPlayerPawn.h"

ARGUnitBase::ARGUnitBase() : bIsSelected(false)
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
	GetMesh()->bReceivesDecals = false;
}

bool ARGUnitBase::GetIsSelected() const
{
	return bIsSelected;
}

int32 ARGUnitBase::GetImportance() const
{
	return UnitImportance;
}

UTexture2D* ARGUnitBase::GetSelectionIcon() const
{
	return SelectionIcon;
}

TArray<UBaseAction*> ARGUnitBase::GetAvailableActions() const
{
	return AvailableActions;
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
	if (!SelectionCircleDecal)
	{
		return;
	}

	bIsSelected = bIsUnitSelected;
	SelectionCircleDecal->SetVisibility(bIsUnitSelected);
}

void ARGUnitBase::AddActionToQueue(UBaseAction* Action) const
{
	ActionQueue->EnqueueAction(Action);
}

bool ARGUnitBase::CanPerformAction(UBaseAction* Action) const
{
	UObject* ActionObj = Cast<UObject>(Action);
	if(!ActionObj)
	{
		return false;
	}
	
	for(UBaseAction* AvailableAction : AvailableActions)
	{
		UObject* AvailableActionObj = Cast<UObject>(AvailableAction);
		if(AvailableActionObj && AvailableActionObj->GetClass() == ActionObj->GetClass())
			return true;
	}
	
	return false;
}

void ARGUnitBase::ClearActionQueue() const
{
	ActionQueue->ClearQueue();
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGUnitBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		return;
	}

	EntityHandler = Cast<ARGPlayerPawn>(PlayerController->GetPawn())->GetEntityHandler();
	if (!EntityHandler)
	{
		return;
	}
	
	ActionQueue = NewObject<UActionQueue>();
	ActionQueue->Initialize(this);
}

void ARGUnitBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerController || !EntityHandler)
	{
		return;
	}

	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		bool bIsShiftDown = PlayerController->IsInputKeyDown(EKeys::LeftShift);
		bool bIsCtrlDown = PlayerController->IsInputKeyDown(EKeys::LeftControl);

		if (!EntityHandler->IsEntitySelected(this))
		{
			if (bIsShiftDown)
			{
				EntityHandler->AddUnitToSelected(this);
			}
			else
			{
				EntityHandler->ClearSelectedEntities();
				EntityHandler->AddUnitToSelected(this);
			}
		}
		else if (bIsCtrlDown)
		{
			EntityHandler->RemoveUnitFromSelected(this);
		}
		else
		{
			EntityHandler->ClearSelectedEntities();
			EntityHandler->AddUnitToSelected(this);
		}
	}
}

void ARGUnitBase::InitializeActions()
{
	UMoveToAction* MoveToAction = NewObject<UMoveToAction>();
	FActionData MoveToData = UnitActions::Base_Move;
	MoveToData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMoveTo.T_IconMoveTo"));
	MoveToAction->InitializeAction(this);
	MoveToAction->SetActionData(MoveToData);

	AvailableActions.Add(MoveToAction);
}