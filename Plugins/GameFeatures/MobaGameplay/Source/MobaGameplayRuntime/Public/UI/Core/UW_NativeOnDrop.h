// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UW_NativeOnDrop.generated.h"

/**
 * 可拖放的UI
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_NativeOnDrop : public UUW_Base
{
	GENERATED_BODY()
public:
	UUW_NativeOnDrop(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
