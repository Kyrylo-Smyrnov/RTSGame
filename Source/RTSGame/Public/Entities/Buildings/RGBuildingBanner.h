// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGBuildingBanner.generated.h"

UCLASS()
class RTSGAME_API ARGBuildingBanner : public AActor
{
	GENERATED_BODY()

  public:
	ARGBuildingBanner();

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;
};
