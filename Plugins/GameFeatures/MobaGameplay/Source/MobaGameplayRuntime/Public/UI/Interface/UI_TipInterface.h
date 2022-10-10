// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/Data/SlotData.h"
#include "UI_TipInterface.generated.h"

class APlayerState;
class UUW_Tip;
class UWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUI_TipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBAGAMEPLAYRUNTIME_API IUI_TipInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IUI_TipInterface();

protected:

	UWidget* VerifyTip(APlayerState* InPlayerState, UUW_Tip* InTip, int32 InSlotID, const FSlotData* ClientSlotData = NULL);
};
