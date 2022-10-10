// Copyright CloudHu. All Rights Reserved.
#include "UI/Core/UW_NativeOnDrop.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/Shop/Item/UW_ItemBase.h"

UUW_NativeOnDrop::UUW_NativeOnDrop(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool UUW_NativeOnDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;

	if (UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUW_ItemBase* Item = Cast<UUW_ItemBase>(InDragDropOperation->Payload))
		{
			Item->UpdateItem();

			bDrop = true;
		}
	}

	return bDrop;
}
