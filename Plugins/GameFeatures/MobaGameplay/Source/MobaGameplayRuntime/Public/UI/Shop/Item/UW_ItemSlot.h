// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Shop/Item/UW_ItemBase.h"
#include "UW_ItemSlot.generated.h"

/**
 * 物品槽
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_ItemSlot : public UUW_ItemBase
{
	GENERATED_BODY()

public:
	UUW_ItemSlot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
};