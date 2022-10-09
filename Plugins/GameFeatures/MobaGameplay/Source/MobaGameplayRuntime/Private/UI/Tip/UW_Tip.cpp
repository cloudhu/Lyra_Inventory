// Copyright CloudHu. All Rights Reserved.
#include "UI/Tip/UW_Tip.h"
#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"

void UUW_Tip::SetDisplayNameText(const FText& InText)
{
	if (InText.IsEmpty())
	{
		NameBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	NameBox->SetVisibility(ESlateVisibility::Visible);
	DisplayName->SetText(InText);
}

void UUW_Tip::SetDescribeText(const FText& InText)
{
	if (InText.IsEmpty())
	{
		DescribeBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	DescribeBox->SetVisibility(ESlateVisibility::Visible);
	DescribeText->SetText(InText);
}

void UUW_Tip::SetTextAction(const FText& InText)
{
	if (InText.IsEmpty())
	{
		ActionBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	ActionBox->SetVisibility(ESlateVisibility::Visible);
	RichTextAction->SetText(InText);
}

void UUW_Tip::SetTextActive(const FText& InText)
{
	if (InText.IsEmpty())
	{
		ActiveBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	ActiveBox->SetVisibility(ESlateVisibility::Visible);
	RichTextActive->SetText(InText);
}

void UUW_Tip::SetTextPassive(const FText& InText)
{
	if (InText.IsEmpty())
	{
		PassiveBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	PassiveBox->SetVisibility(ESlateVisibility::Visible);
	RichTextPassive->SetText(InText);
}

void UUW_Tip::SetTextSoul(const FText& InText)
{
	if (InText.IsEmpty())
	{
		SoulBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	SoulBox->SetVisibility(ESlateVisibility::Visible);
	SoulText->SetText(InText);
}
