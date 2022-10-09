// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UW_Tip.generated.h"

class UCommonTextBlock;
class UHorizontalBox;
/**
 * 提示
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_Tip : public UUW_Base
{
	GENERATED_BODY()
public:

	//名称
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* DisplayName;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* NameBox;

	//描述
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* DescribeText;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* DescribeBox;

	//动作
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* RichTextAction;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* ActionBox;

	//主动效果
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* RichTextActive;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* ActiveBox;

	//被动效果
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* RichTextPassive;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* PassiveBox;

	//售价
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* SoulText;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* SoulBox;
public:

	//UFUNCTION(BlueprintCallable, Category = Tip)
		void SetDisplayNameText(const FText& InText);

	//UFUNCTION(BlueprintCallable, Category = Tip)
	void SetDescribeText(const FText& InText);

	//UFUNCTION(BlueprintCallable, Category = Tip)
	void SetTextAction(const FText& InText);

	//UFUNCTION(BlueprintCallable, Category = Tip)
	void SetTextActive(const FText& InText);

	//UFUNCTION(BlueprintCallable, Category = Tip)
	void SetTextPassive(const FText& InText);

	void SetTextSoul(const FText& InText);
};
