// Copyright CloudHu. All Rights Reserved.


#include "UI/Data/SlotData.h"

FSlotData::FSlotData()
	:SlotID(INDEX_NONE)
	,SlotICO(NULL)
	,CD(0.f)
	,Number(INDEX_NONE)
	,bCancelBuy(true)
{
}

void FSlotData::Reset()
{
	bCancelBuy = true;
	SlotID = INDEX_NONE;
	SlotICO = NULL;
	CD = 0.f;
	Number = INDEX_NONE;
}
