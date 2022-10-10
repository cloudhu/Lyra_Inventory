// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlotData.generated.h"

/**
 * 槽数据
 */
USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_USTRUCT_BODY()

		FSlotData();

		UPROPERTY()
		int32 SlotID;

	UPROPERTY()
		UTexture2D* SlotICO;

	UPROPERTY()
		float CD;

	UPROPERTY()
		int32 Number;

	UPROPERTY()
		bool bCancelBuy;

	void Reset();
};