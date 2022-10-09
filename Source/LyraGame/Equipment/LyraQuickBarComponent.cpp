// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraQuickBarComponent.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "Equipment/LyraEquipmentManagerComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Inventory/InventoryFragment_ConsumableItem.h"
#include "Player/LyraPlayerController.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Inventory/LyraInventoryManagerComponent.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_QuickBar_Message_SlotsChanged, "Lyra.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_QuickBar_Message_ActiveIndexChanged, "Lyra.QuickBar.Message.ActiveIndexChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Inventory_Item_Count, "Lyra.Inventory.Item.Count");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Inventory_Message_StackChanged, "Lyra.Inventory.Message.StackChanged");
ULyraQuickBarComponent::ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void ULyraQuickBarComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void ULyraQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

void ULyraQuickBarComponent::SwapItemSlot(int32 SlotIndexA, int32 SlotIndexB)
{
	//1.确定索引在有效范围
	if (Slots.IsValidIndex(SlotIndexA) && Slots.IsValidIndex(SlotIndexB))
	{
		//2.交换数据
		ULyraInventoryItemInstance* ItemA = Slots[SlotIndexA];
		Slots[SlotIndexA] = Slots[SlotIndexB];
		Slots[SlotIndexB] = ItemA;

		//3.更新UI
		OnRep_Slots();

		//4.如果是激活的索引,则替换激活的索引
		if (ActiveSlotIndex == SlotIndexA)
		{
			SetActiveSlotIndex(SlotIndexB);
		}
		else if (ActiveSlotIndex == SlotIndexB)
		{
			SetActiveSlotIndex(SlotIndexA);
		}
	}
}

void ULyraQuickBarComponent::UnequipItem(ULyraInventoryItemInstance* SlotItem)
{
	if (SlotItem)
	{
		if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
		{
			if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
			{
				TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
				if (EquipDef != nullptr)
				{
					if (const ULyraEquipmentDefinition* EquipmentCDO = GetDefault<ULyraEquipmentDefinition>(EquipDef))
					{
						TSubclassOf<ULyraEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
						if (InstanceType != nullptr)
						{
							if (ULyraEquipmentInstance* EquipmentInstance = EquipmentManager->GetFirstInstanceOfType(InstanceType))
							{
								EquipmentManager->UnequipItem(EquipmentInstance);
								if (EquippedItem == EquipmentInstance)
								{
									EquippedItem = nullptr;
								}
							}
						}
					}
				}
			}
		}
	}
}

void ULyraQuickBarComponent::EquipItem(ULyraInventoryItemInstance* SlotItem)
{
	if (SlotItem)
	{
		if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
		{
			TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					if (ULyraEquipmentInstance* EquipmentInstance = EquipmentManager->EquipItem(EquipDef))
					{
						EquipmentInstance->SetInstigator(SlotItem);
						if (EquippedItem == nullptr)
						{
							EquippedItem = EquipmentInstance;
						}
					}
				}
			}
		}
	}
}

void ULyraQuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void ULyraQuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void ULyraQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));

	if (ULyraInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		//新增消耗品的使用
		if (const UInventoryFragment_ConsumableItem* ConsumableInfo = SlotItem->FindFragmentByClass<UInventoryFragment_ConsumableItem>())
		{
			if (ALyraPlayerController* OwnerController = Cast<ALyraPlayerController>(GetOwner()))
			{
				if (ULyraAbilitySystemComponent* ASC = OwnerController->GetLyraAbilitySystemComponent())
				{
					TSubclassOf<UGameplayEffect> CostGameplayEffect = ConsumableInfo->CostGameplayEffectClass;
					if (CostGameplayEffect != nullptr)
					{
						FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffect, 1.0f, ASC->MakeEffectContext());

						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						int32 StackCount = SlotItem->GetStatTagStackCount(TAG_Lyra_Inventory_Item_Count);
						if (ULyraInventoryManagerComponent* InventoryComponent = OwnerController->FindComponentByClass<ULyraInventoryManagerComponent>())
						{
							if (StackCount == 1)
							{
								InventoryComponent->RemoveItemInstance(SlotItem);
								RemoveItemFromSlot(ActiveSlotIndex);
							}
							else
							{
								SlotItem->RemoveStatTagStack(TAG_Lyra_Inventory_Item_Count, 1);
								//通知UI更新
								FLyraInventoryChangeMessage Message;
								Message.InventoryOwner = InventoryComponent;
								Message.Instance = SlotItem;
								Message.NewCount = StackCount - 1;
								Message.Delta = 1;

								UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(InventoryComponent->GetWorld());
								MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
							}
						}
						return;
					}
				}
			}
		}

		check(EquippedItem == nullptr);
		if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
		{
			TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

void ULyraQuickBarComponent::UnequipItemInSlot()
{
	if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

ULyraEquipmentManagerComponent* ULyraQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void ULyraQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

ULyraInventoryItemInstance* ULyraQuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 ULyraQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (TObjectPtr<ULyraInventoryItemInstance> ItemPtr : Slots)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

void ULyraQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

ULyraInventoryItemInstance* ULyraQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	ULyraInventoryItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void ULyraQuickBarComponent::OnRep_Slots()
{
	FLyraQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = Slots;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Lyra_QuickBar_Message_SlotsChanged, Message);
}

void ULyraQuickBarComponent::OnRep_ActiveSlotIndex()
{
	FLyraQuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = ActiveSlotIndex;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Lyra_QuickBar_Message_ActiveIndexChanged, Message);
}
