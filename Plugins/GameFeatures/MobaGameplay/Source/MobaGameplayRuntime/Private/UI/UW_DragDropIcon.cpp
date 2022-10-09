// Copyright (C) CloudHu.2022.All Rights Reserved.
#include "UI/UW_DragDropIcon.h"
#include "Components/Image.h"

UUW_DragDropIcon::UUW_DragDropIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_DragDropIcon::DrawICON(UTexture2D* InICON)
{
	if (ICON)
	{
		ICON->SetBrushFromTexture(InICON);
	}
}
