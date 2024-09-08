// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "RGBTTaskAttack.generated.h"

UCLASS()
class RTSGAME_API URGBTTaskAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
