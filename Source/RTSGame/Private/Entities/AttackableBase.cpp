// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "Entities/AttackableBase.h"
#include "Components/DecalComponent.h"

AAttackableBase::AAttackableBase(int32 InHealth) : Health(InHealth), bIsDecalVisible(false)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetVisibility(false);
	DecalComponent->SetupAttachment(Root);
}

void AAttackableBase::ReceiveDamage(float DamageAmount, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		Destroy();
	}
}

void AAttackableBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAttackableBase::StartDecalBlinking()
{
	GetWorld()->GetTimerManager().SetTimer(DecalBlinkTimerHandle, this, &AAttackableBase::BlinkDecal, 0.25f, true);
}

void AAttackableBase::BlinkDecal()
{
	bIsDecalVisible = !bIsDecalVisible;
	DecalComponent->SetVisibility(bIsDecalVisible);

	static int32 BlinkCount = 0;
	BlinkCount++;

	if (BlinkCount >= 4)
	{
		StopDecalBlinking();
		BlinkCount = 0;
	}
}

void AAttackableBase::StopDecalBlinking()
{
	GetWorld()->GetTimerManager().ClearTimer(DecalBlinkTimerHandle);
	DecalComponent->SetVisibility(false);
	bIsDecalVisible = false;
}