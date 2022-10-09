// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/Item/UW_InventorySlot.h"
#include "UI/UW_DragDropIcon.h"
#include "Equipment/LyraQuickBarComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"

UUW_InventorySlot::UUW_InventorySlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, SlotIndex(INDEX_NONE)
{
}

void UUW_InventorySlot::UpdateInventorySlot(TSubclassOf<ULyraInventoryItemDefinition> ItemDef)
{
	if (ItemDef)
	{
		if (UMobaPlayerStateComponent* MobaPlayerStateComponent = FindMobaPlayerStateComponent())
		{//找到保存数据的PSC组件,再通过PSC的函数根据物品定义拿到数据,根据数据更新背包格子
			if (const FSlotTable* slot = MobaPlayerStateComponent->GetItemByDef(ItemDef))
			{
				UpdateSlot(slot);
				UpdateTipSoul(slot->SlotSoul * 0.5);
			}
		}
	}
	else
	{
		UpdateSlot(nullptr);
	}

}

void UUW_InventorySlot::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

FReply UUW_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{//如果是鼠标右键按住,说明玩家在拖拽背包格子
		if (GetSlotTable() && GetSlotTable()->IsValid())//仅拖拽有数据的格子
		{
			FReply Reply = FReply::Handled();
			TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
			if (SlateWidgetDrag.IsValid())
			{//触发拖拽的事件
				Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
				return Reply;
			}
		}
	}

	return FReply::Handled();
}

void UUW_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (DragDropIconClass)//这个类生成拖拽的图标
	{
		if (const FSlotTable* InSlotData = GetSlotTable())//获取背包格子对应的数据缓存
		{
			if (InSlotData->IsValid())
			{
				if (UUW_DragDropIcon* DragDropIcon = CreateWidget<UUW_DragDropIcon>(GetWorld(), DragDropIconClass))
				{//生成拖拽的图标
					if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass()))
					{//生成拖放的操作
						InDropOperation->SetFlags(RF_StrongRefOnFrame);
						DragDropIcon->DrawICON(InSlotData->SlotIcon);//赋值给图标
						InDropOperation->DefaultDragVisual = DragDropIcon;
						InDropOperation->Payload = this;
						OutOperation = InDropOperation;
						//拖拽过程中关闭背包格子内的图标显示
						ShowIcon(false);
					}
				}
			}
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUW_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	bool bDrop = false;
	//这里就是放下的操作了,如果是背包内的拖拽,那么可以理解成两个背包格子之间的交换位置
	if (UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUW_InventorySlot* DraggedInventorySlot = Cast<UUW_InventorySlot>(InDragDropOperation->Payload))
		{//现在只支持背包格子之间的拖拽
			if (UMobaPlayerStateComponent* PSC = FindMobaPlayerStateComponent())
			{
				if (ULyraQuickBarComponent* QuickBarComponent = PSC->GetQuickBarComponent())
				{//获取Lyra的快捷栏组件,这里暂时使用的是快捷栏的数据,实际上这是不正确的,快捷栏和背包是两个概念,在后续重制的独立背包插件中解决此问题
					DraggedInventorySlot->ShowIcon(true);
					QuickBarComponent->SwapItemSlot(SlotIndex, DraggedInventorySlot->GetSlotIndex());
					bDrop = true;
				}
			}
		}
	}

	return bDrop;
}