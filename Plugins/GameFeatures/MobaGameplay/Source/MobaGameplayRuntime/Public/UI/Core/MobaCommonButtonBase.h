// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "MobaCommonButtonBase.generated.h"

/**
 * 按钮基类
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UMobaCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void SetButtonText(const FText& InText);

protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface

	void RefreshButtonText();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateButtonStyle();

private:
	UPROPERTY(EditAnywhere, Category = "Button", meta = (InlineEditConditionToggle))
		uint8 bOverride_ButtonText : 1;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (editcondition = "bOverride_ButtonText"))
		FText ButtonText;
};