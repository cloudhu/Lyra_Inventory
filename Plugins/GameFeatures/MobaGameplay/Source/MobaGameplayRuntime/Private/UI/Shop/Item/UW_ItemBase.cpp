// Copyright (C) CloudHu.2022.All Rights Reserved.
#include "UI/Shop/Item/UW_ItemBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/Tip/UW_Tip.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "UI/Data/MobaVerbMessage.h"
#include "PlayerStateExtension/MobaPlayerStateComponent.h"

void UUW_ItemBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_ItemBase::SetIcon(UTexture2D* InIcon2D)
{
	if (InIcon2D)
	{
		ItemIcon->SetBrushFromTexture(InIcon2D);
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_ItemBase::SetIconBrush(const FSlateBrush& Brush)
{
	ItemIcon->SetBrush(Brush);
}

void UUW_ItemBase::BuyItem()
{
	if (UMobaPlayerStateComponent* PSC = FindMobaPlayerStateComponent())
	{
		Enabled(false);//防止玩家快速点击
		PSC->Buy(ItemID);
	}
}

void UUW_ItemBase::SellItem(int32 SlotIndex)
{
	if (UMobaPlayerStateComponent* PSC = FindMobaPlayerStateComponent())
	{
		Enabled(false);//防止玩家快速点击
		PSC->Sell(ItemID, SlotIndex);
	}
}

TSubclassOf<ULyraInventoryItemDefinition> UUW_ItemBase::GetItemDef()
{
	return FindMobaPlayerStateComponent()->GetItemDef(ItemID);
}

bool UUW_ItemBase::IsConsumable()
{
	if (Table)
	{
		return Table->IsConsumable();
	}
	return false;
}

void UUW_ItemBase::SetItemID(int32 InID)
{
	ItemID = InID;
}

UUW_ItemBase::UUW_ItemBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, ItemID(INDEX_NONE)
	, Soul(INDEX_NONE)
	, Table(nullptr)
{

}

void UUW_ItemBase::UpdateSlot(const FSlotTable* InTable)
{
	if (InTable)
	{
		ItemButton->SetToolTip(GetTip());
		SetItemID(InTable->ID);
		SetIcon(InTable->SlotIcon);
		SetSlotTable(InTable);
		SetItemSoulNum(InTable->SlotSoul);
		UpdateTip(InTable);
	}
	else
	{
		ItemButton->SetToolTip(nullptr);
		SetItemID(INDEX_NONE);
		SetIcon(nullptr);
		SetSlotTable(nullptr);
		SetItemSoulNum(0);
		UpdateTip(nullptr);
	}
}

void UUW_ItemBase::UpdateItem()
{
	if (Table)
	{
		UpdateSlot(Table);
	}
	else
	{
		UpdateSlot(nullptr);
	}
}

void UUW_ItemBase::SetItemSoulNum(int32 SoulNum)
{
	Soul = SoulNum;
	UpdateTipSoul(SoulNum);
}

void UUW_ItemBase::SetSlotTable(const FSlotTable* InTable)
{
	Table = InTable;
}

int32 UUW_ItemBase::GetItemSoulNum()
{
	if (Table)
	{
		if (Table->IsConsumable())//如果是消耗品,直接返回原价
		{
			return Soul;
		}

		if (UMobaPlayerStateComponent* PSC = FindMobaPlayerStateComponent())
		{
			if (PSC->IsInventoryFull())//如果背包已经满了,返回一个超大值使玩家无法购买
			{
				return INT_FAST32_MAX;
			}
			else
			{//计算物品实际所需的消费数量
				SetItemSoulNum(PSC->CalculateSoulNumber(Table));
			}
		}
	}
	return Soul;
}

void UUW_ItemBase::Enabled(bool bIsEnable)
{
	if (ItemButton)
	{
		ItemButton->SetIsEnabled(bIsEnable);
	}

	if (ItemIcon)
	{
		ItemIcon->SetIsEnabled(bIsEnable);
	}
}

const FSlotTable* UUW_ItemBase::GetSlotTable()
{
	return Table;
}

void UUW_ItemBase::ShowIcon(bool bIsVis)
{
	if (bIsVis)
	{
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}
