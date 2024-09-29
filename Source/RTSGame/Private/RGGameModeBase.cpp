// https://github.com/Kyrylo-Smyrnov/RTSGame

#include "RGGameModeBase.h"
#include "Player/RGPlayerController.h"
#include "Player/RGPlayerPawn.h"
#include "Player/UI/RGPlayerHUD.h"

ARGGameModeBase::ARGGameModeBase()
{
	PlayerControllerClass = ARGPlayerController::StaticClass();
	DefaultPawnClass = ARGPlayerPawn::StaticClass();
	HUDClass = ARGPlayerHUD::StaticClass();
}
