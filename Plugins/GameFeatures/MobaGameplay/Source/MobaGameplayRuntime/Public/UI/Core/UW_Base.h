// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Data/SlotTable.h"
#include "Blueprint/UserWidget.h"
#include "UW_Base.generated.h"

class UUW_Tip;
/**
 * UI基类
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_Base : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUW_Tip> TipClass;
public:
	UUW_Base(const FObjectInitializer& ObjectInitializer);//构造函数

	//根据名称获得UI动画
	class UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;

	//获取PlayerState的方便函数
	class APlayerState* GetPlayerState();

	//获取物品数据的方便函数
	const FSlotTable* GetSlotTableTemplate(int32 InID);

	//获取物品列表
	const TArray<FSlotTable*>* GetSlotTablesTemplate();

	//获取魂力数量
	float GetPlayerSoulNum();
public:
	//获取数据源PlayerStateComponent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = DataSource)
	class UMobaPlayerStateComponent* FindMobaPlayerStateComponent();

	//获取提示信息
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
		virtual UWidget* GetTip();

public:
	//更新提示信息
	void UpdateTip(const FSlotTable* InTable);

	//更新魂力提示数量
	void UpdateTipSoul(int32 SoulNum);
protected:
	//缓存提示组件
	UPROPERTY()
		UUW_Tip* TipPtr;

};
