// Copyright (C) CloudHu.2022.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UW_ItemBase.generated.h"

class UImage;
class UButton;
/**
 * 物品基类
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_ItemBase : public UUW_Base
{
	GENERATED_BODY()
private:
	int32 ItemID;//物品编号

	int32 Soul;//魂力数量

	const FSlotTable* Table;//数据缓存
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* ItemIcon;//物品图标

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* ItemButton;//物品按钮
protected:
	virtual void NativeConstruct() override;//UI原生构造

public:
	UUW_ItemBase(const FObjectInitializer& ObjectInitializer);//构造函数

	virtual void UpdateSlot(const FSlotTable* InTable);//根据数据更新UI显示

	void UpdateItem();//更新物品

	void SetIcon(UTexture2D* InIcon2D);//设置图标

	void SetItemID(int32 InID);//设置编号

	void SetItemSoulNum(int32 SoulNum);//设置物品魂力数量

	void SetSlotTable(const FSlotTable* InTable);//设置数据缓存

	int32 GetItemSoulNum();//获取物品魂力数量

	virtual void Enabled(bool bIsEnable);//激活物品

	const FSlotTable* GetSlotTable();//获取物品数据

	//是否显示图标
	void ShowIcon(bool bIsVis);
public:
	//设置物品图标显示
	void SetIconBrush(const FSlateBrush& Brush);

	UFUNCTION(BlueprintCallable, Category = Item)
		void BuyItem();//购买物品

	UFUNCTION(BlueprintCallable, Category = Item)
		void SellItem(int32 SlotIndex);//出售物品

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Item)
		TSubclassOf<ULyraInventoryItemDefinition> GetItemDef();//获取物品定义

	//是否是消耗品
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Item)
		bool IsConsumable();
public:
	FORCEINLINE int32 GetItemID() const { return ItemID; }//获取物品编号
};
