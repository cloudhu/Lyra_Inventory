#include "UI/Data/SlotTable.h"
#include "Inventory/LyraInventoryItemDefinition.h"

FSlotTable::FSlotTable()
	:ID(INDEX_NONE)
	, SlotSoul(10)
{
	SlotType.Add(ESlotType::SLOT_ALL);
}

bool FSlotTable::IsValid() const
{
	return ID != INDEX_NONE;
}

bool FSlotTable::IsConsumable() const
{
	return SlotType.Contains(ESlotType::SLOT_CONSUMABLES);
}

