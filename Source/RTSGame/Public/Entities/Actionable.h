// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Actionable.generated.h"

USTRUCT(BlueprintType)
struct FActionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	FName ActionName = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	UTexture2D* ActionIcon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	FText ActionTooltip = FText::FromString("<Default tooltip>");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionManaCost = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionGoldCost = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionWoodCost = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionCooldown = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 UnitSpawnTime = 0;

	FActionData()
	{
	}
	FActionData(FName ActionName, UTexture2D* ActionIcon, FText ActionTooltip, int32 ActionManaCost,
				int32 ActionGoldCost, int32 ActionWoodCost, int32 ActionCooldown, int32 UnitSpawnTime)
		: ActionName(ActionName), ActionIcon(ActionIcon), ActionTooltip(ActionTooltip), ActionManaCost(ActionManaCost),
		  ActionGoldCost(ActionGoldCost), ActionWoodCost(ActionWoodCost), ActionCooldown(ActionCooldown), UnitSpawnTime(UnitSpawnTime)
	{
	}
};

UINTERFACE(MinimalAPI)
class UActionable : public UInterface
{
	GENERATED_BODY()
};

class IActionable
{
	GENERATED_BODY()

  public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	TArray<FActionData> GetAvailableActions() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	void PerformAction(const FName& ActionName);
};