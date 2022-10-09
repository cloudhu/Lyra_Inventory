// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/Item/UW_Item.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

UUW_Item::UUW_Item(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}


FReply UUW_Item::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		CallEquipmentSynthesisDelegate.ExecuteIfBound(GetItemID());
	}

	return FReply::Handled();
}

void UUW_Item::SetItemName(const FText& InName)
{
	ItemName->SetText(InName);
}

void UUW_Item::SetItemSoul(const int32 InSoul)
{
	SoulText->SetText(FText::Format(NSLOCTEXT("UUW_Item", "SetItemSoul", "{0}"), InSoul));
}

void UUW_Item::UpdateSlot(const FSlotTable* InTable)
{
	Super::UpdateSlot(InTable);
	if (InTable)
	{
		SetItemName(InTable->DisplayName);
		SetItemSoul(InTable->SlotSoul);
	}
}

void UUW_Item::Enabled(bool bIsEnable)
{
	Super::Enabled(bIsEnable);

	if (SoulBox)
	{
		SoulBox->SetIsEnabled(bIsEnable);
	}
}
