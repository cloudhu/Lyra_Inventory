// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayEffect.h"
#include "SlotTable.generated.h"
class ULyraInventoryItemDefinition;
UENUM(BlueprintType)
enum class ESlotType :uint8
{
	SLOT_ALL					UMETA(DisplayName = "All"),
	//基础装备
	SLOT_BASIC					UMETA(DisplayName = "Basic"),
	//史诗
	SLOT_EPIC					UMETA(DisplayName = "Epic"),
	//传说
	SLOT_LEGENDARY				UMETA(DisplayName = "Legendary"),
	//神话
	SLOT_MYTHIC					UMETA(DisplayName = "Mythic"),
	//消耗品
	SLOT_CONSUMABLES			UMETA(DisplayName = "Consumables"),
	//辅助装备
	SLOT_AUXILIARYPROPS			UMETA(DisplayName = "AuxiliaryProps"),
	//物理攻击
	SLOT_PHYSICALATTACK			UMETA(DisplayName = "PhysicalAttack"),
	//魔法攻击
	SLOT_MAGICATTACK			UMETA(DisplayName = "MagicAttack"),
	//物理防御
	SLOT_PHYSICALPENETRATION	UMETA(DisplayName = "PhysicalPenetration"),
	//魔法防御
	SLOT_MAGICPENETRATION		UMETA(DisplayName = "MagicPenetration"),
	//暴击
	SLOT_CRIT					UMETA(DisplayName = "Crit"),
	//护甲
	SLOT_ARMOR					UMETA(DisplayName = "Armor"),
	//魔法防御
	SLOT_MAGICRESISTANCE		UMETA(DisplayName = "MagicResistance"),
	//攻击速度
	SLOT_ATTACKSPEED			UMETA(DisplayName = "AttackSpeed"),
	//鞋子
	SLOT_SHOES					UMETA(DisplayName = "Shoes"),
};


USTRUCT(BlueprintType)
struct FSlotTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
		FSlotTable();

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		int32 SlotSoul;
	//名称
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FText DisplayName;
	//简介
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FText SlotIntroduction;
	//动作
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FText RichTextAction;

	//主动效果
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FText RichTextActive;

	//被动效果
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		FText RichTextPassive;

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		UTexture2D* SlotIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		TArray<ESlotType> SlotType;

	//装备合成
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
		TArray<int32> Childrens;

	/** 对应Lyra的背包物品 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slot)
		TSubclassOf<ULyraInventoryItemDefinition> ItemDefinition;

	/** 消耗品的GE */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Slot)
		TSubclassOf<UGameplayEffect> CostGameplayEffectClass;
public:
	TSubclassOf<ULyraInventoryItemDefinition> GetItemDef() const
	{
		return ItemDefinition;
	}

	bool IsValid() const;

	//是否是消耗品,如果是消耗品,可以叠加5个,其他物品无法叠加,消耗品可以在满格的情况下购买,并且购买后直接消耗掉
	bool IsConsumable() const;
};