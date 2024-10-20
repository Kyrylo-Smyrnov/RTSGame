// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGTargetTypeActorAction.generated.h"

UINTERFACE(MinimalAPI)
class URGTargetTypeActorAction : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IRGTargetTypeActorAction
{
	GENERATED_BODY()

public:
	virtual void SetTarget(AActor* InTarget) = 0;
};

