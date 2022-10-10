// Copyright CloudHu. All Rights Reserved.


#include "UI/Interface/UI_TipInterface.h"

// Add default functionality here for any IUI_TipInterface functions that are not pure virtual.

IUI_TipInterface::IUI_TipInterface()
{

}

UWidget* IUI_TipInterface::VerifyTip(APlayerState* InPlayerState, UUW_Tip* InTip, int32 InSlotID, const FSlotData* ClientSlotData /*= NULL*/)
{
	//if (InPlayerState && InTip && InSlotID != INDEX_NONE)
	//{
	//	if (!ClientSlotData)
	//	{
	//		ClientSlotData = InPlayerState->GetSlotData(InSlotID);
	//	}

	//	if (ClientSlotData)
	//	{
	//		if (ClientSlotData->SlotID != INDEX_NONE)
	//		{
	//			if (const FSlotTable* InSlotTable = InPlayerState->GetSlotTableTemplate(ClientSlotData->SlotID))
	//			{
	//				if (const FSlotAttribute* InSlotAttribute = InPlayerState->GetSlotAttributeTemplate(InSlotTable->ID))
	//				{
	//					//��ֵ����
	//					InTip->SetDescribeText(InSlotTable->SlotIntroduction);

	//					MethodUnit::TipRegistration(InTip, InSlotAttribute);
	//				}
	//			}

	//			return InTip;
	//		}
	//	}
	//}

	return NULL;
}
