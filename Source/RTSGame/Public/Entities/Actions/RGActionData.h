// https://github.com/Kyrylo-Smyrnov/RTSGame

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "RGActionData.generated.h"

USTRUCT(BlueprintType)
struct FRGActionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	FName ActionName;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	UTexture2D* ActionIcon;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	FText ActionTooltip;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionManaCost;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionGoldCost;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionWoodCost;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 ActionCooldown;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Action")
	int32 TimeToProduce;
	
	FRGActionData()
		: ActionName(NAME_None), ActionIcon(nullptr), ActionTooltip(FText::FromString("<Default tooltip>")),
		  ActionManaCost(0), ActionGoldCost(0), ActionWoodCost(0), ActionCooldown(0), TimeToProduce(0) {}

	FRGActionData(FName Name, UTexture2D* Icon, FText Tooltip, int32 ManaCost, int32 GoldCost, int32 WoodCost,
				int32 Cooldown, int32 SpawnTime)
		: ActionName(Name), ActionIcon(Icon), ActionTooltip(Tooltip), ActionManaCost(ManaCost),
		  ActionGoldCost(GoldCost), ActionWoodCost(WoodCost), ActionCooldown(Cooldown), TimeToProduce(SpawnTime) {}
};