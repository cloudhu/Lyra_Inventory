// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_Base.h"
#include "UI/Data/SlotTable.h"
#include "UW_ItemCompound.generated.h"
class UCanvasPanel;
class UUW_ItemSlot;

typedef uint8 FSlotLayer;
USTRUCT(BlueprintType)
struct FDrawEquipLine
{
	GENERATED_USTRUCT_BODY()

		// | 
		UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* VerticalLine;

	// _
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* HorizontalLine;

	//_|_
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeftAndRightVertically;

	//|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnRight;

	//-|
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeft;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TLine;
};

/**
 * 物品组合
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UUW_ItemCompound : public UUW_Base
{
	GENERATED_BODY()
		UPROPERTY(meta = (BindWidget))
		UCanvasPanel* EquipmentPanel;

	UPROPERTY(EditDefaultsOnly, Category = Pram)
		FVector2D StartPostion;

	UPROPERTY(EditDefaultsOnly, Category = Pram)
		FVector2D IconSize;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUW_ItemSlot> ItemSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		FDrawEquipLine LineTexture;
public:
	UUW_ItemCompound(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//UFUNCTION(BlueprintCallable)
	void UpdateSlot(const FSlotTable* InTable);

	//UFUNCTION(BlueprintCallable)
	void UpdateAllItem();

	//UFUNCTION(BlueprintCallable)
	void CheckSoul(const FSlotTable* InTable, UUW_ItemSlot* InSlot);
protected:

	int32 GetLayerDepth(const FSlotTable* InTable, int32 InDepth = 1);

	void RecursiveUpdateSlot(const FSlotTable* InTable, const FVector2D& InStartPostion, FSlotLayer InSlotLayer);
};
