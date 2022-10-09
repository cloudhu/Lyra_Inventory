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

	FSlotData* GetSlotData(int32 InSlotID);

	const TArray<FSlotTable*>* GetSlotTablesTemplate();

	const FSlotTable* GetSlotTableTemplate(int32 InID);

	const FSlotTable* GetItemByDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef);

	//计算购买物品需要消耗的数量
	int32 CalculateSoulNumber(const FSlotTable* InTable, bool bIsServer = false);

	ULyraInventoryManagerComponent* GetInventoryManagerComponent() const;
	ULyraQuickBarComponent* GetQuickBarComponent() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	float GetPlayerSoulNum();

	bool ModifySoulNum(TEnumAsByte<EGameplayModOp::Type> ModifyMode, int32 Value);
	//背包是否已经满格
	bool IsInventoryFull();
public:
	UFUNCTION(BlueprintPure, Category = "Moba|PlayerState")
		static UMobaPlayerStateComponent* FindMobaPlayerStateComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMobaPlayerStateComponent>() : nullptr); }

	//UFUNCTION(BlueprintCallable, Category = Inventory)
	TSubclassOf<ULyraInventoryItemDefinition> GetItemDef(int32 InID);

	//UFUNCTION(BlueprintCallable, Category = Inventory)
	const FSlotTable& GetItemByInventoryDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef);
public:
	UPROPERTY(EditDefaultsOnly, Category = "Player data")
		TMap<int32, FSlotData> InventorySlots; //TMap

	UFUNCTION(Server, reliable)
		void Buy(int32 InSlotID);

	UFUNCTION(Server, reliable)
		void Use(int32 InSlotID, int32 InSlotIndex);

	UFUNCTION(Server, reliable)
		void Sell(int32 InSlotID, int32 InSlotIndex);

	UFUNCTION(Server, reliable)
		void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass);

private:
	//递归获取合成商品所需的物品编号
	void RecursivelyGetItemID(const FSlotTable* InTable, TArray<int32>& ItemIds);

	void RemoveItem(int32 slotIndex, int32 StackCount = 1);
protected:
	UPROPERTY()
		UDataTable* SlotTablePtr;
private:
	TArray<FSlotTable*> CacheSlotTables;
};
