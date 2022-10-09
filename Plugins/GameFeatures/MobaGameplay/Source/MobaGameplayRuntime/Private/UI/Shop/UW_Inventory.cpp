// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_Inventory.h"
#include "CommonTextBlock.h"
//#include "Components/UniformGridPanel.h"
//#include "Components/CanvasPanelSlot.h"
//#include "Blueprint/WidgetTree.h"
//#include "Components/UniformGridSlot.h"
//#include "UI/Shop/Item/UW_ItemSlot.h"
//#include "GameFramework/PlayerController.h"
//#include "Inventory/LyraInventoryItemDefinition.h"
//#include "Inventory/LyraInventoryManagerComponent.h"
//#include "Inventory/LyraInventoryItemInstance.h"

UUW_Inventory::UUW_Inventory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UUW_Inventory::UpdateMyInventory()
{
	SetSoulText();
	UpdateAllItem();
}

void UUW_Inventory::SetSoulText()
{
	SoulText->SetText(FText::Format(NSLOCTEXT("UUW_Item", "SetItemSoul", "{0}"), GetPlayerSoulNum()));
}

//void UUW_Inventory::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//}

//void UUW_Inventory::UpdateMyInventory()
//{
//	SoulText->SetText(FText::Format(NSLOCTEXT("UUW_Item", "SetItemSoul", "{0}"), GetPlayerSoulNum()));
//#if 0
//	if (APlayerController* PC = GetOwningPlayer())
//	{
//		if (ULyraInventoryManagerComponent* InventoryComponent = PC->FindComponentByClass<ULyraInventoryManagerComponent>())
//		{
//			TArray<ULyraInventoryItemInstance*> ItemArray = InventoryComponent->GetAllItems();
//			if (ItemArray.Num>0)
//			{
//
//			}
//		}
//	}
//#endif
//
//}
