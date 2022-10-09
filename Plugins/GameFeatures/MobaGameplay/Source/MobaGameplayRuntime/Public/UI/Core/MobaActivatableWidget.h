// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "CommonActivatableWidget.h"
#include "MobaActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EMobaWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};
/**
 * 可通过技能激活的UI
 */
UCLASS(Abstract, Blueprintable)
class MOBAGAMEPLAYRUNTIME_API UMobaActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UMobaActivatableWidget(const FObjectInitializer& ObjectInitializer);

public:

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
		EMobaWidgetInputMode InputConfig = EMobaWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
		EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
