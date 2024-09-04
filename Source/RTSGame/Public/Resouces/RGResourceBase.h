// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGResourceBase.generated.h"

UCLASS()
class RTSGAME_API ARGResourceBase : public AActor
{
	GENERATED_BODY()

  public:
	ARGResourceBase();
	virtual void Tick(float DeltaTime) override;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
};
