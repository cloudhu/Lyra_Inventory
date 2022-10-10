// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UI/Data/SlotTable.h"
#include "UW_ItemCompound.generated.h"
class UCanvasPanel;
class UUW_ItemSlot;

typedef uint8 FSlotLayer;
USTRUCT(BlueprintType)
struct FDrawEquipLine//绘制合成装备线条的结构
{
	GENERATED_USTRUCT_BODY()

		// | 垂直线
		UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* VerticalLine;

	// _ 水平线
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* HorizontalLine;

	//_|_ 
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeftAndRightVertically;

	//|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnRight;

	//-|
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeft;
	// T 
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TLine;
};

/**
 * 物品组合
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_ItemCompound : public UUW_Base
{
	GENERATED_BODY()
		UPROPERTY(meta = (BindWidget))
		UCanvasPanel* EquipmentPanel;//装备显示面板

	UPROPERTY(EditDefaultsOnly, Category = Pram)
		FVector2D StartPostion;//起始位置

	UPROPERTY(EditDefaultsOnly, Category = Pram)
		FVector2D IconSize;//图标尺寸

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUW_ItemSlot> ItemSlotClass;//物品类

	UPROPERTY(EditDefaultsOnly, Category = UI)
		FDrawEquipLine LineTexture;//线条贴图
public:
	UUW_ItemCompound(const FObjectInitializer& ObjectInitializer);//构造函数

	virtual void NativeConstruct() override;

	//根据数据更新物品槽,这里主要针对合成物品所需的子类
	void UpdateSlot(const FSlotTable* InTable);

	void UpdateAllItem();//更新所有物品

	//检查魂力
	void CheckSoul(const FSlotTable* InTable, UUW_ItemSlot* InSlot);
protected:
	//获取物品合成的深度,也就是要盖几层楼
	int32 GetLayerDepth(const FSlotTable* InTable, int32 InDepth = 1);

	//递归更新合成的UI显示
	void RecursiveUpdateSlot(const FSlotTable* InTable, const FVector2D& InStartPostion, FSlotLayer InSlotLayer);
};
