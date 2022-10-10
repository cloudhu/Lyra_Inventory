// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Shop/Item/UW_ItemBase.h"
#include "UW_Item.generated.h"
class UImage;
class UCommonTextBlock;
struct FSlotTable;
class UHorizontalBox;
//声明调用物品合成的代理,传入物品编号作为参数
DECLARE_DELEGATE_OneParam(FCallEquipmentSynthesisDelegate, int32)

/**
 * 物品
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_Item : public UUW_ItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* SoulText;//魂力数量的文本显示

	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* ItemName;//物品名称

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* SoulBox;//魂力盒子
public:
	UUW_Item(const FObjectInitializer& ObjectInitializer);//构造函数

	FCallEquipmentSynthesisDelegate CallEquipmentSynthesisDelegate;//代理,这个在Shop中生成物品时进行绑定
protected:
	//在这里完成鼠标右键的判定,通过该事件来通知合成面板展示合成信息
	virtual	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	virtual void NativeConstruct() override;

public:
	void SetItemName(const FText& InName);//设置物品名称
	void SetItemSoul(const int32 InSoul);//设置魂力数量

	virtual void UpdateSlot(const FSlotTable* InTable) override;//重写父类的更新函数

	void Enabled(bool bIsEnable) override;//重写父类的激活函数
};
