// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateComponent.h"
#include "UI/Data/SlotData.h"
#include "UI/Data/SlotTable.h"
#include "MobaPlayerStateComponent.generated.h"

class ULyraInventoryManagerComponent;
class ULyraInventoryItemDefinition;
class ULyraInventoryItemInstance;
class ULyraQuickBarComponent;
class UAbilitySystemComponent;
/**
 * Moba玩法的PS扩展组件
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UMobaPlayerStateComponent : public UPlayerStateComponent
{
	GENERATED_BODY()

public:
	UMobaPlayerStateComponent(const FObjectInitializer& ObjectInitializer);

	//获取所有的物品数据
	const TArray<FSlotTable*>* GetSlotTablesTemplate();
	//通过ID编号获取物品数据
	const FSlotTable* GetSlotTableTemplate(int32 InID);
	//通过物品定义来获取物品数据
	const FSlotTable* GetItemByDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef);

	//计算购买物品需要消耗的魂力数量
	int32 CalculateSoulNumber(const FSlotTable* InTable, bool bIsServer = false);
	//获取背包管理组件
	ULyraInventoryManagerComponent* GetInventoryManagerComponent() const;
	//获取快捷栏组件
	ULyraQuickBarComponent* GetQuickBarComponent() const;
	//获取ASC组件
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//获取玩家魂力数量
	float GetPlayerSoulNum();
	//修改魂力数量
	bool ModifySoulNum(TEnumAsByte<EGameplayModOp::Type> ModifyMode, int32 Value);
	//背包是否已经满格
	bool IsInventoryFull();
public:
	//找到PSC组件
	UFUNCTION(BlueprintPure, Category = "Moba|PlayerState")
		static UMobaPlayerStateComponent* FindMobaPlayerStateComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMobaPlayerStateComponent>() : nullptr); }

	//根据ID来获取物品定义
	TSubclassOf<ULyraInventoryItemDefinition> GetItemDef(int32 InID);

	//根据定义来获取物品数据
	const FSlotTable& GetItemByInventoryDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef);
public:
	//购买,服务器端执行
	UFUNCTION(Server, reliable)
		void Buy(int32 InSlotID);

	//使用
	UFUNCTION(Server, reliable)
		void Use(int32 InSlotID, int32 InSlotIndex);

	//出售
	UFUNCTION(Server, reliable)
		void Sell(int32 InSlotID, int32 InSlotIndex);

	//消耗品效果GE
	UFUNCTION(Server, reliable)
		void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass);

private:
	//递归获取合成商品所需的物品编号
	void RecursivelyGetItemID(const FSlotTable* InTable, TArray<int32>& ItemIds);

	//移除物品
	void RemoveItem(int32 slotIndex, int32 StackCount = 1);
protected:
	//物品配置表指针
	UPROPERTY()
		UDataTable* SlotTablePtr;
private:
	//配置表缓存数据
	TArray<FSlotTable*> CacheSlotTables;
};
