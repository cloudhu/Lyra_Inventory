// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UW_Inventory.generated.h"

class UCommonTextBlock;

/**
 * 游戏内商店背包
 */
UCLASS(Abstract, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_Inventory : public UUW_Base
{
	GENERATED_BODY()

public:
	UUW_Inventory(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* SoulText;

public:
	UFUNCTION(BlueprintCallable, Category = MobaInventory)
		void UpdateMyInventory();

	void SetSoulText();

	UFUNCTION(BlueprintImplementableEvent, Category = MobaInventory)
		void UpdateAllItem();
};
