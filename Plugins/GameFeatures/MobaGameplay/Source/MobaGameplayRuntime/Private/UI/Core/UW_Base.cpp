// Copyright CloudHu. All Rights Reserved.
#include "UI/Core/UW_Base.h"
#include "Animation/WidgetAnimation.h"
#include "UI/Tip/UW_Tip.h"

UUW_Base::UUW_Base(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

UWidgetAnimation* UUW_Base::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}

	return nullptr;
}

UWidget* UUW_Base::GetTip()
{
	if (!TipPtr)
	{
		if (TipClass)
		{
			TipPtr = CreateWidget<UUW_Tip>(GetWorld(), TipClass);
		}
	}

	return TipPtr;
}

void UUW_Base::UpdateTip(const FSlotTable* InTable)
{
	if (InTable)
	{
		GetTip();
		TipPtr->SetDisplayNameText(InTable->DisplayName);
		TipPtr->SetDescribeText(InTable->SlotIntroduction);
		TipPtr->SetTextAction(InTable->RichTextAction);
		TipPtr->SetTextActive(InTable->RichTextActive);
		TipPtr->SetTextPassive(InTable->RichTextPassive);
		TipPtr->SetTextSoul(FText::Format(NSLOCTEXT("UUW_Item", "SetItemSoul", "{0}"), InTable->SlotSoul));
	}
}

void UUW_Base::UpdateTipSoul(int32 SoulNum)
{
	if (TipPtr)
	{
		TipPtr->SetTextSoul(FText::Format(NSLOCTEXT("UUW_Item", "SetItemSoul", "{0}"), SoulNum));
	}
}

APlayerState* UUW_Base::GetPlayerState()
{
	return GetOwningPlayerState<APlayerState>();
}

const FSlotTable* UUW_Base::GetSlotTableTemplate(int32 InID)
{
	return FindMobaPlayerStateComponent()->GetSlotTableTemplate(InID);
}

const TArray<FSlotTable*>* UUW_Base::GetSlotTablesTemplate()
{
	return FindMobaPlayerStateComponent()->GetSlotTablesTemplate();
}

float UUW_Base::GetPlayerSoulNum()
{
	if (UMobaPlayerStateComponent* PSC = FindMobaPlayerStateComponent())
	{
		return PSC->GetPlayerSoulNum();
	}

	return 0.f;
}

UMobaPlayerStateComponent* UUW_Base::FindMobaPlayerStateComponent()
{
	return UMobaPlayerStateComponent::FindMobaPlayerStateComponent(GetPlayerState());
}