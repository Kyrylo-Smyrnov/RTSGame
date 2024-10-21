// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/Units/RGUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Entities/Actions/ActionQueue.h"
#include "Entities/Actions/Implementation/MoveToAction.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RGPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGUnitBase, All, All);

ARGUnitBase::ARGUnitBase()
	: bIsSelected(false)
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bReceivesDecals = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	SelectionCircleDecal = CreateDefaultSubobject<UDecalComponent>("SelectionCircleDecal");
	SelectionCircleDecal->SetupAttachment(GetRootComponent());
	SelectionCircleDecal->SetVisibility(false);
}

bool ARGUnitBase::GetIsSelected() const
{
	return bIsSelected;
}

void ARGUnitBase::SetSelected(bool bIsUnitSelected)
{
	if (!SelectionCircleDecal)
	{
		UE_LOG(LogRGUnitBase, Warning, TEXT("[SetSelected] SelectionCircleDecal is nullptr."));
		return;
	}

	bIsSelected = bIsUnitSelected;
	SelectionCircleDecal->SetVisibility(bIsUnitSelected);
}

int32 ARGUnitBase::GetImportance() const
{
	return UnitImportance;
}

UTexture2D* ARGUnitBase::GetSelectionIcon() const
{
	if (!SelectionIcon)
		UE_LOG(LogRGUnitBase, Warning, TEXT("[GetSelectionIcon] SelectionIcon is nullptr."))

	return SelectionIcon;
}

TArray<UBaseAction*> ARGUnitBase::GetAvailableActions()
{
	return AvailableActions;
}

void ARGUnitBase::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ARGUnitBase::HandleOnClicked);

	PlayerController = Cast<ARGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		UE_LOG(LogRGUnitBase, Warning, TEXT("[BeginPlay] PlayerController is nullptr."));
		return;
	}

	PlayerPawn = Cast<ARGPlayerPawn>(PlayerController->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogRGUnitBase, Warning, TEXT("[BeginPlay] PlayerPawn is nullptr."));
		return;
	}

	InitializeActions();
	ActionQueue = NewObject<UActionQueue>();
	ActionQueue->Initialize(this);
}

void ARGUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARGUnitBase::HandleOnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (!PlayerController || !PlayerPawn)
	{
		UE_LOG(LogRGUnitBase, Warning, TEXT("[HandleOnClicked] PlayerController or PlayerPawn is nullptr."))
		return;
	}

	if (ButtonPressed == EKeys::LeftMouseButton)
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

void ARGUnitBase::InitializeActions()
{
	UMoveToAction* MoveToAction = NewObject<UMoveToAction>();
	FActionData MoveToData = UnitActions::Base_Move;
	MoveToData.ActionIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Icons/Entities/Units/T_IconMoveTo.T_IconMoveTo"));
	MoveToAction->InitializeAction(this);
	MoveToAction->SetActionData(MoveToData);

	AvailableActions.Add(MoveToAction);
}

void ARGUnitBase::AddActionToQueue(UBaseAction* Action) const
{
	ActionQueue->EnqueueAction(Action);
}

void ARGUnitBase::ClearActionQueue() const
{
	ActionQueue->ClearQueue();
}

bool ARGUnitBase::CanPerformAction(UBaseAction* Action)
{
	UObject* ActionObj = Cast<UObject>(Action);
	if(!ActionObj)
	{
		UE_LOG(LogRGUnitBase, Warning, TEXT("[CanPerformAction] Failed to cast Action -> UObject"));
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