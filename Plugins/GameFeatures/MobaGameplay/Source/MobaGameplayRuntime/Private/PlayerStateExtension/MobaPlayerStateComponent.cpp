// Copyright CloudHu. All Rights Reserved.
#include "PlayerStateExtension/MobaPlayerStateComponent.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "GAS/AttributeSet/MobaAttributeSet.h"
#include "Equipment/LyraQuickBarComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
//物品数量更新消息GT
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Inventory_Message_StackChanged, "Lyra.Inventory.Message.StackChanged");
//背包物品数量GT
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Inventory_Item_Count, "Lyra.Inventory.Item.Count");

UMobaPlayerStateComponent::UMobaPlayerStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//DataTable'/MobaGameplay/Data/DT_SlotTable_MobaShop.DT_SlotTable_MobaShop' 读取配置表到指针
	static ConstructorHelpers::FObjectFinder<UDataTable> Slot_Table(TEXT("/MobaGameplay/Data/DT_SlotTable_MobaShop"));
	SlotTablePtr = Slot_Table.Object;
}

const TArray<FSlotTable*>* UMobaPlayerStateComponent::GetSlotTablesTemplate()
{
	if (!CacheSlotTables.Num())//如果没有缓存则读取配置表到缓存
	{
		if (SlotTablePtr)
		{
			SlotTablePtr->GetAllRows(TEXT("Slot Tables"), CacheSlotTables);
		}
	}

	return &CacheSlotTables;
}

const FSlotTable* UMobaPlayerStateComponent::GetSlotTableTemplate(int32 InID)
{
	for (auto& Tmp : *GetSlotTablesTemplate())
	{
		if (Tmp->ID == InID)
		{
			return Tmp;
		}
	}

	return NULL;
}

TSubclassOf<ULyraInventoryItemDefinition> UMobaPlayerStateComponent::GetItemDef(int32 InID)
{
	if (const FSlotTable* slot = GetSlotTableTemplate(InID))
	{
		return slot->ItemDefinition;
	}
	return NULL;
}

const FSlotTable& UMobaPlayerStateComponent::GetItemByInventoryDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	return *GetItemByDef(ItemDef);
}

void UMobaPlayerStateComponent::Buy_Implementation(int32 InSlotID)
{
	if (const FSlotTable* InTable = GetSlotTableTemplate(InSlotID))
	{
		if (ULyraQuickBarComponent* QuickBarComponent = GetQuickBarComponent())
		{
			int32 SlotIndex = QuickBarComponent->GetNextFreeItemSlot();
			if (SlotIndex != INDEX_NONE)
			{
				//1.计算消耗的数量
				int32 CostSoulNum = CalculateSoulNumber(InTable, true);
				if (GetPlayerSoulNum() >= CostSoulNum)
				{//2.扣除消费
					if (ModifySoulNum(EGameplayModOp::Additive, -CostSoulNum))
					{
						if (ULyraInventoryManagerComponent* InventoryComponent = GetInventoryManagerComponent())
						{
							ULyraInventoryItemInstance* Result = InventoryComponent->FindFirstItemStackByDefinition(InTable->ItemDefinition);
							if (InTable->IsConsumable())//是否是消耗品
							{
								if (Result)
								{
									//修改数量
									int32 StackCount = Result->GetStatTagStackCount(TAG_Lyra_Inventory_Item_Count);
									Result->AddStatTagStack(TAG_Lyra_Inventory_Item_Count, StackCount + 1);
									//通知UI更新
									FLyraInventoryChangeMessage Message;
									Message.InventoryOwner = InventoryComponent;
									Message.Instance = Result;
									Message.NewCount = StackCount;
									Message.Delta = 1;

									UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(InventoryComponent->GetWorld());
									MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
									return;
								}
							}
							//直接添加到背包,更新UI界面
							Result = InventoryComponent->AddItemDefinition(InTable->ItemDefinition);
							QuickBarComponent->AddItemToSlot(SlotIndex, Result);
							QuickBarComponent->EquipItem(Result);
						}
					}
				}
			}
			else if (InTable->IsConsumable())//如果没有多余的空格,那么就只能购买消耗品,并且在购买之后立即被消耗
			{
				if (GetPlayerSoulNum() >= InTable->SlotSoul)
				{
					if (ModifySoulNum(EGameplayModOp::Additive, -(InTable->SlotSoul)))
					{
						//应用消耗品的效果
						ApplyGameplayEffect(InTable->CostGameplayEffectClass);
					}
				}
			}
		}
	}
}

void UMobaPlayerStateComponent::Use_Implementation(int32 InSlotID, int32 InSlotIndex)
{
	if (const FSlotTable* InTable = GetSlotTableTemplate(InSlotID))
	{
		if (ULyraInventoryManagerComponent* InventoryComponent = GetInventoryManagerComponent())
		{
			if (ULyraInventoryItemInstance* Result = InventoryComponent->FindFirstItemStackByDefinition(InTable->ItemDefinition))
			{
				if (InTable->IsConsumable())//如果是消耗品
				{
					RemoveItem(InSlotIndex, Result->GetStatTagStackCount(TAG_Lyra_Inventory_Item_Count));
					//应用消耗品的效果
					ApplyGameplayEffect(InTable->CostGameplayEffectClass);
				}
			}
		}
	}
}

void UMobaPlayerStateComponent::Sell_Implementation(int32 InSlotID, int32 InSlotIndex)
{
	if (const FSlotTable* InTable = GetSlotTableTemplate(InSlotID))
	{//出售的逻辑比较简单,五折收购,移除物品
		if (ULyraInventoryManagerComponent* InventoryComponent = GetInventoryManagerComponent())
		{
			if (ULyraInventoryItemInstance* Result = InventoryComponent->FindFirstItemStackByDefinition(InTable->ItemDefinition))
			{
				if (ModifySoulNum(EGameplayModOp::Additive, 0.5f * (InTable->SlotSoul)))
				{
					if (ULyraQuickBarComponent* QuickBarComponent = GetQuickBarComponent())
					{
						QuickBarComponent->UnequipItem(Result);
					}
					RemoveItem(InSlotIndex, Result->GetStatTagStackCount(TAG_Lyra_Inventory_Item_Count));
				}
			}
		}
	}
}

void UMobaPlayerStateComponent::ApplyGameplayEffect_Implementation(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (UAbilitySystemComponent* theASC = GetAbilitySystemComponent())
	{
		FGameplayEffectSpecHandle SpecHandle = theASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, theASC->MakeEffectContext());

		theASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UMobaPlayerStateComponent::RecursivelyGetItemID(const FSlotTable* InTable, TArray<int32>& ItemIds)
{
	for (auto& Tmp : InTable->Childrens)
	{
		if (const FSlotTable* InSlotTable = GetSlotTableTemplate(Tmp))
		{
			ItemIds.Add(InSlotTable->ID);
			if (InSlotTable->Childrens.Num() != 0)
			{
				RecursivelyGetItemID(InSlotTable, ItemIds);
			}
		}
	}
}

void UMobaPlayerStateComponent::RemoveItem(int32 slotIndex, int32 StackCount)
{
	if (ULyraQuickBarComponent* QuickBarComponent = GetQuickBarComponent())
	{
		if (ULyraInventoryManagerComponent* InventoryComponent = GetInventoryManagerComponent()) {
			if (StackCount == 1)
			{
				InventoryComponent->RemoveItemInstance(QuickBarComponent->RemoveItemFromSlot(slotIndex));
			}
			else
			{
				TArray<ULyraInventoryItemInstance*> InventoryItems = QuickBarComponent->GetSlots();//背包中的物品
				ULyraInventoryItemInstance* Result = InventoryItems[slotIndex];
				Result->RemoveStatTagStack(TAG_Lyra_Inventory_Item_Count, 1);
				//通知UI更新
				FLyraInventoryChangeMessage Message;
				Message.InventoryOwner = InventoryComponent;
				Message.Instance = Result;
				Message.NewCount = StackCount - 1;
				Message.Delta = 1;

				UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(InventoryComponent->GetWorld());
				MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
			}
		}
	}
}

const FSlotTable* UMobaPlayerStateComponent::GetItemByDef(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	for (const FSlotTable* slot : *GetSlotTablesTemplate())
	{
		if (slot->ItemDefinition == ItemDef)
		{
			return slot;
		}
	}
	return NULL;
}

int32 UMobaPlayerStateComponent::CalculateSoulNumber(const FSlotTable* InTable, bool bIsServer)
{
	int32 NeedSoulNumber = 9999999;
	if (!InTable)
	{
		return NeedSoulNumber;
	}

	//原本消耗的数量
	NeedSoulNumber = InTable->SlotSoul;
	if (InTable->IsConsumable())
	{
		return NeedSoulNumber;//如果是消耗品,直接返回原价
	}
	//I	 递归获取合成所需的基础物品编号
	TArray<int32> IteamIDs;
	RecursivelyGetItemID(InTable, IteamIDs);

	//II  获取已经购买了的合成所需的物品数据
	if (ULyraQuickBarComponent* QuickBarComponent = GetQuickBarComponent())
	{
		TArray<ULyraInventoryItemInstance*> InventoryItems = QuickBarComponent->GetSlots();//背包中的物品
		TArray<int32> RemoveItems;//需要移除的物品
		for (auto& ItemID : IteamIDs)
		{
			for (int32 i = 0; i < InventoryItems.Num(); i++)
			{
				if (!RemoveItems.Contains(i))
				{
					if (ULyraInventoryItemInstance* Item = InventoryItems[i])
					{
						if (const FSlotTable* InSlotTable = GetItemByDef(Item->GetItemDef()))
						{
							if (ItemID == InSlotTable->ID)
							{
								NeedSoulNumber -= InSlotTable->SlotSoul;//III 减去已经购买了的
								RemoveItems.Add(i);
								break;
							}
						}
					}
				}
			}
		}

		//服务器上执行
#if WITH_SERVER_CODE
	//I  移除物品子项
		if (bIsServer)//加这个判断的原因是在编辑器运行时,编辑器既是客户端,又是权威
		{
			if (ULyraInventoryManagerComponent* InventoryComponent = GetInventoryManagerComponent())
			{
				for (int32 slotIndex : RemoveItems)
				{
					InventoryComponent->RemoveItemInstance(QuickBarComponent->RemoveItemFromSlot(slotIndex));
				}
			}
		}
#endif
	}
	return NeedSoulNumber;
}

ULyraInventoryManagerComponent* UMobaPlayerStateComponent::GetInventoryManagerComponent() const
{
	ULyraInventoryManagerComponent* InventoryComponent = nullptr;
	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		if (APlayerController* PC = PS->GetPlayerController())
		{
			InventoryComponent = PC->FindComponentByClass<ULyraInventoryManagerComponent>();
		}
	}
	return InventoryComponent;
}

ULyraQuickBarComponent* UMobaPlayerStateComponent::GetQuickBarComponent() const
{
	ULyraQuickBarComponent* QuickBarComponent = nullptr;
	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		if (APlayerController* PC = PS->GetPlayerController())
		{
			QuickBarComponent = PC->FindComponentByClass<ULyraQuickBarComponent>();
		}
	}
	return QuickBarComponent;
}

UAbilitySystemComponent* UMobaPlayerStateComponent::GetAbilitySystemComponent() const
{
	if (APlayerState* InPlayerState = GetPlayerState<APlayerState>())
	{
		return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPlayerState);
	}
	return nullptr;
}

float UMobaPlayerStateComponent::GetPlayerSoulNum()
{
	if (UAbilitySystemComponent* theASC = GetAbilitySystemComponent())
	{
		return theASC->GetNumericAttribute(UMobaAttributeSet::GetSoulAttribute());
	}

	return 0.f;
}

bool UMobaPlayerStateComponent::ModifySoulNum(TEnumAsByte<EGameplayModOp::Type> ModifyMode, int32 Value)
{
	if (UAbilitySystemComponent* theASC = GetAbilitySystemComponent())
	{
		theASC->ApplyModToAttribute(UMobaAttributeSet::GetSoulAttribute(), ModifyMode, Value);
		return true;
	}
	return false;
}

bool UMobaPlayerStateComponent::IsInventoryFull()
{
	if (ULyraQuickBarComponent* QuickBarComponent = GetQuickBarComponent())
	{
		return (QuickBarComponent->GetNextFreeItemSlot() == INDEX_NONE);
	}
	return true;
}
