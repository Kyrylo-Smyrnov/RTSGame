// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "RGGameModeBase.h"
#include "RGPlayerController.h"
#include "Player/RGPlayerPawn.h"
#include "Player/RGPlayerHUD.h"

ARGGameModeBase::ARGGameModeBase()
{
	PlayerControllerClass = ARGPlayerController::StaticClass();
	DefaultPawnClass = ARGPlayerPawn::StaticClass();
	HUDClass = ARGPlayerHUD::StaticClass();
}
