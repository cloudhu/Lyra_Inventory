// Copyright (C) CloudHu.2022.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_DragDropIcon.generated.h"

class UImage;
/**
 * 拖拽的图标
 */
UCLASS(Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UUW_DragDropIcon : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage *ICON;

public:
	UUW_DragDropIcon(const FObjectInitializer& ObjectInitializer);
	void DrawICON(UTexture2D *InICON);
};
