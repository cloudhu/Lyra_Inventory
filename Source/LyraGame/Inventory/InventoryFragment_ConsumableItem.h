// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "GameplayEffect.h"
#include "InventoryFragment_ConsumableItem.generated.h"

UCLASS()
class UInventoryFragment_ConsumableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Lyra)
		TSubclassOf<UGameplayEffect> CostGameplayEffectClass;
};
