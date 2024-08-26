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

	FActionData()
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