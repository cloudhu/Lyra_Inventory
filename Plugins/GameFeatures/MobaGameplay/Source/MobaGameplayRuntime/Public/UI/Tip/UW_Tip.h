// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Tip.generated.h"

class UCommonTextBlock;
class UHorizontalBox;
/**
 * 提示
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_Tip : public UUserWidget
{
	GENERATED_BODY()
public:

	//名称
	UPROPERTY(meta = (BindWidget))
		UCommonTextBlock* DisplayName;

	//名称盒子
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
	void SetDisplayNameText(const FText& InText);

	void SetDescribeText(const FText& InText);

	void SetTextAction(const FText& InText);

	void SetTextActive(const FText& InText);

	void SetTextPassive(const FText& InText);

	void SetTextSoul(const FText& InText);
};
