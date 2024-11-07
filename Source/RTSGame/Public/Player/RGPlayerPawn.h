// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RGPlayerPawn.generated.h"

class UEntityHandlerComponent;
class UPlayerCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerResourcesChanged, int32);

UCLASS()
class RTSGAME_API ARGPlayerPawn : public APawn
{
	GENERATED_BODY()

  public:
	ARGPlayerPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UEntityHandlerComponent* GetEntityHandler() const;

	void AddPlayerResources(int32 Amount);
	int32 GetPlayerResources() const;

	FOnPlayerResourcesChanged OnPlayerResourcesChanged;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UPlayerCameraComponent* CameraComponent;
	UPROPERTY()
	UEntityHandlerComponent* EntityHandler;

  private:
	int32 PlayerResources;
};
