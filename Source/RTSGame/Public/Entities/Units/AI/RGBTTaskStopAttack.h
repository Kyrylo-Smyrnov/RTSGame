// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGBTTaskStopAttack.generated.h"

/**
 * 
 */
UCLASS()
class RTSGAME_API URGBTTaskStopAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
