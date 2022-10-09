// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/LyraNotificationMessage.h"
#include "UW_Shop.generated.h"
class UCheckBox;
class UUniformGridPanel;
class UUW_Item;
enum class ESlotType :uint8;
class UUW_ItemCompound;

/**
 * 游戏内商店
 */
UCLASS(Abstract, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_Shop : public UUW_Base
{
	GENERATED_BODY()

public:
	UUW_Shop(const FObjectInitializer& ObjectInitializer);//构造函数

	UPROPERTY(meta = (BindWidget))
		UCheckBox* All;//所有品类商品

	UPROPERTY(meta = (BindWidget))
		UCheckBox* Consumables;//消耗品

	UPROPERTY(meta = (BindWidget))
		UCheckBox* AuxiliaryProps;//辅助类商品

	UPROPERTY(meta = (BindWidget))
		UCheckBox* PhysicalAttack;//物理攻击

	UPROPERTY(meta = (BindWidget))
		UCheckBox* MagicAttack;//魔法攻击

	UPROPERTY(meta = (BindWidget))
		UCheckBox* PhysicalPenetration;//物理穿透

	UPROPERTY(meta = (BindWidget))
		UCheckBox* MagicPenetration;//魔法穿透

	UPROPERTY(meta = (BindWidget))
		UCheckBox* Crit;//暴击

	UPROPERTY(meta = (BindWidget))
		UCheckBox* Armor;//护甲

	UPROPERTY(meta = (BindWidget))
		UCheckBox* MagicResistance;//魔法抗性

	UPROPERTY(meta = (BindWidget))
		UCheckBox* AttackSpeed;//攻击速度

	UPROPERTY(meta = (BindWidget))
		UCheckBox* Shoes;//鞋子

	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* ItemGrid;//物品列表

	UPROPERTY(meta = (BindWidget))
		UUW_ItemCompound* EquipmentCombination;//合成菜单

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUW_Item> ItemClass;//物品类

public:
	void HandleSoulNumChanged(const FOnAttributeChangeData& ChangeData);//处理魂力数量变化

	void BindSoulNumChanged();//绑定魂力变化
public:
	virtual void NativeConstruct() override;//UI原生构造

	//Tick,UI原生更新
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	void UpdateItem(ESlotType Type);//更新物品
protected:
	UFUNCTION(BlueprintCallable,Category=Shop)
	void UpdateAll();//更新所有商品

	UFUNCTION(BlueprintCallable, Category = Shop)
		void OnCloseWidget();//关闭商店界面

	UFUNCTION(BlueprintCallable, Category = Shop)
		void OnNativeKey();//原生按键触发打开商店界面

	UFUNCTION(BlueprintImplementableEvent, Category = Shop)
		void OnCloseWidgetComplete();//商店关闭后通知其他界面

	UFUNCTION()
		void OnCallUpdateEquipmentCombination(int32 InItemSlotID);//更新物品合成界面
protected://不同的勾选,展示不同的商品
	UFUNCTION()
		void CheckBoxAll(bool bIsChecked);

	UFUNCTION()
		void CheckBoxConsumables(bool bIsChecked);

	UFUNCTION()
		void CheckBoxAuxiliaryProps(bool bIsChecked);

	UFUNCTION()
		void CheckBoxPhysicalAttack(bool bIsChecked);

	UFUNCTION()
		void CheckBoxMagicAttack(bool bIsChecked);

	UFUNCTION()
		void CheckBoxPhysicalPenetration(bool bIsChecked);

	UFUNCTION()
		void CheckBoxMagicPenetration(bool bIsChecked);

	UFUNCTION()
		void CheckBoxCrit(bool bIsChecked);

	UFUNCTION()
		void CheckBoxArmor(bool bIsChecked);

	UFUNCTION()
		void CheckBoxMagicResistance(bool bIsChecked);

	UFUNCTION()
		void CheckBoxAttackSpeed(bool bIsChecked);

	UFUNCTION()
		void CheckBoxShoes(bool bIsChecked);

protected:
	void SetCheckBoxArray(ECheckBoxState CheckBoxState);//设置勾选框状态

	bool IsScopeStore();//是否在商店有效范围内
protected:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);//鼠标移动原生事件
	virtual	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);//鼠标按下
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);//鼠标松开

private:
	//勾选框数组
	TArray<UCheckBox*> CheckBoxArray;
	//界面偏移
	FVector2D Offset;
	//商店位置缓存
	FVector ShopLocation;
	//更新频率记录
	float Time;
};
