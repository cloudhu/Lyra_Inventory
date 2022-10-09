// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Shop/Item/UW_ItemBase.h"
#include "UW_InventorySlot.generated.h"

/**
 * 背包物品槽
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_InventorySlot : public UUW_ItemBase
{
	GENERATED_BODY()
private:
	int32 SlotIndex;//背包格子索引
public:
	//生成的拖拽图标
	UPROPERTY(EditDefaultsOnly, Category = InventorySlot)
		TSubclassOf<class UUW_DragDropIcon> DragDropIconClass;

public:
	UUW_InventorySlot(const FObjectInitializer& ObjectInitializer);//构造函数

	FORCEINLINE int32 GetSlotIndex() { return SlotIndex; }//获取索引
public:
	UFUNCTION(BlueprintCallable, Category = InventorySlot)
		void UpdateInventorySlot(TSubclassOf<ULyraInventoryItemDefinition> ItemDef);//更新背包格子
	UFUNCTION(BlueprintCallable, Category = InventorySlot)
		void SetSlotIndex(int32 Index);//设置背包索引
protected:
	//下面的都是UI的原生事件,根据名称就知道是什么事件了
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//检测到背包格子被拖拽
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	//检测到背包格子被放下
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};