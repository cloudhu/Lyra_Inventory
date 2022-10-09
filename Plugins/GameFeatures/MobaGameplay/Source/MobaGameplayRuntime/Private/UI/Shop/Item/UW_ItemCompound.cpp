// Copyright GanBowen. All Rights Reserved.
#include "UI/Shop/Item/UW_ItemCompound.h"
#include "UI/Shop/Item/UW_ItemSlot.h"
#include "PlayerStateExtension/MobaPlayerStateComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

struct FDrawSlot
{
	//
	struct FSlot
	{
		FSlot(UCanvasPanelSlot* InSlot)
			:Slot(InSlot)
		{}

		void SetPostion(const FVector2D& InPostion);
		FVector2D GetSize() const;
	private:
		UCanvasPanelSlot* Slot;
	};

	struct FTable :public FSlot
	{
		FTable(UCanvasPanelSlot* InSlot, const FSlotTable* InTableSlot)
			:FSlot(InSlot)
			, TableSlot(InTableSlot)
		{}

		const FSlotTable* GetSlotTable();
	private:
		const FSlotTable* TableSlot;
	};

	TArray<FSlot> LREndPanel;
	TArray<FSlot> VLinePanel;
	TArray<FSlot> HLinePanel;
	TArray<FSlot> TLinePanel;
	TArray<FSlot> LRVPanel;
	TArray<FTable> ItemPanel;
};

void FDrawSlot::FSlot::SetPostion(const FVector2D& InPostion)
{
	if (Slot)
	{
		Slot->SetPosition(InPostion);
	}
}

FVector2D FDrawSlot::FSlot::GetSize() const
{
	return Slot != NULL ? Slot->GetSize() : FVector2D::ZeroVector;
}

const FSlotTable* FDrawSlot::FTable::GetSlotTable()
{
	return TableSlot;
}

UUW_ItemCompound::UUW_ItemCompound(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_ItemCompound::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_ItemCompound::UpdateSlot(const FSlotTable* InTable)
{
	EquipmentPanel->ClearChildren();

	if (InTable)
	{
		//
		int32 InLayer = GetLayerDepth(InTable);
		if (UUW_ItemSlot* InMainSlotWidget = CreateWidget<UUW_ItemSlot>(GetWorld(), ItemSlotClass))
		{

			if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InMainSlotWidget))
			{
				InPanelSlot->SetSize(IconSize);
				InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));
				InPanelSlot->SetPosition(StartPostion);

				RecursiveUpdateSlot(InTable, StartPostion, --InLayer);
			}
			InMainSlotWidget->UpdateSlot(InTable);
			CheckSoul(InTable, InMainSlotWidget);
		}
	}
}

void UUW_ItemCompound::UpdateAllItem()
{
	for (auto& Tmp : EquipmentPanel->GetAllChildren())
	{
		if (UUW_ItemSlot* InItem = Cast<UUW_ItemSlot>(Tmp))
		{
			if (GetPlayerSoulNum() < InItem->GetItemSoulNum())
			{
				InItem->SetIsEnabled(false);
			}
			else
			{
				InItem->SetIsEnabled(true);
			}
		}
	}
}

void UUW_ItemCompound::CheckSoul(const FSlotTable* InTable, UUW_ItemSlot* InSlot)
{
	if (InTable && InSlot)
	{
		if (InSlot->GetItemSoulNum() > GetPlayerSoulNum())
		{
			InSlot->SetIsEnabled(false);
		}
		else
		{
			InSlot->SetIsEnabled(true);
		}
	}
}

int32 UUW_ItemCompound::GetLayerDepth(const FSlotTable* InTable, int32 InDepth /*= 1*/)
{
	InDepth++;

	TArray<int32> Depths;
	for (auto& Tmp : InTable->Childrens)
	{
		if (const FSlotTable* InSlotTable = GetSlotTableTemplate(Tmp))
		{
			Depths.Add(GetLayerDepth(InSlotTable, InDepth));
		}
	}

	for (auto& Tmp : Depths)
	{
		if (Tmp > InDepth)
		{
			InDepth = Tmp;
		}
	}

	return InDepth;
}

void UUW_ItemCompound::RecursiveUpdateSlot(const FSlotTable* InTable, const FVector2D& InStartPostion, FSlotLayer InSlotLayer)
{
	if (InSlotLayer < 0)
	{
		InSlotLayer = 0;
	}

	int32 NumberChildrens = InTable->Childrens.Num();

	FDrawSlot DrawSlot;
	auto SpawnSlot = [&](TArray<FDrawSlot::FSlot>& PanelSlots, UTexture2D* InTexute, int32 InLayer = 1)
	{
		if (UImage* InImage = NewObject<UImage>(this))
		{
			InImage->SetBrushFromTexture(InTexute);
			if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InImage))
			{
				InPanelSlot->SetSize(FVector2D(IconSize.X * InLayer, IconSize.Y));
				InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));

				PanelSlots.Add(FDrawSlot::FSlot(InPanelSlot));
			}
		}
	};

	auto SpawnItem = [&](TArray<FDrawSlot::FTable>& PanelSlots, const FSlotTable* InTable)
	{
		if (UUW_ItemSlot* InMainSlotWidget = CreateWidget<UUW_ItemSlot>(GetWorld(), ItemSlotClass))
		{

			if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InMainSlotWidget))
			{
				InPanelSlot->SetSize(IconSize);
				InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));

				PanelSlots.Add(FDrawSlot::FTable(InPanelSlot, InTable));
			}
			InMainSlotWidget->UpdateSlot(InTable);
			CheckSoul(InTable, InMainSlotWidget);
		}
	};

	//
//////////////////////////////////////////////////////////////////////////
	if (NumberChildrens == 1)
	{
		SpawnSlot(DrawSlot.VLinePanel, LineTexture.VerticalLine);
	}
	else if (NumberChildrens >= 2)
	{
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnRight);
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnLeft);

		SpawnSlot(DrawSlot.LRVPanel, LineTexture.TurnLeftAndRightVertically);

		//T
		for (int32 i = 0; i < NumberChildrens - 2; i++)
		{
			SpawnSlot(DrawSlot.TLinePanel, LineTexture.TLine);
		}

		for (int32 i = 0; i < NumberChildrens; i++)
		{
			SpawnSlot(DrawSlot.HLinePanel, LineTexture.HorizontalLine, InSlotLayer);
		}
	}

	//
	for (auto& Tmp : InTable->Childrens)
	{
		if (const FSlotTable* InSlotTable = GetSlotTableTemplate(Tmp))
		{
			SpawnItem(DrawSlot.ItemPanel, InSlotTable);
		}
	}

	int32 ValidNumebr = DrawSlot.HLinePanel.Num() +
		DrawSlot.LREndPanel.Num() +
		DrawSlot.LRVPanel.Num() +
		DrawSlot.TLinePanel.Num() +
		DrawSlot.VLinePanel.Num();

	//
	if (NumberChildrens == 1)
	{
		FDrawSlot::FSlot InVLin = DrawSlot.VLinePanel.Pop();
		FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop();
		FVector2D NewPos = FVector2D(InStartPostion.X, InStartPostion.Y + InVLin.GetSize().Y);
		InVLin.SetPostion(NewPos);
		NewPos = FVector2D(NewPos.X, NewPos.Y + InVLin.GetSize().Y);
		InItem.SetPostion(NewPos);
	}
	else if (NumberChildrens >= 2)
	{
		FDrawSlot::FSlot InLRVLin = DrawSlot.LRVPanel.Pop();
		FVector2D NewPos = FVector2D(InStartPostion.X, InStartPostion.Y + InLRVLin.GetSize().Y);
		InLRVLin.SetPostion(NewPos);

		float HWith = DrawSlot.HLinePanel[0].GetSize().X;
		FVector2D R = NewPos;
		FVector2D L = NewPos;

		auto Add = [&](int32 i, TArray<FDrawSlot::FSlot>& InPanels, FSlotLayer InSlotLayer)
		{
			FVector2D InNewValidPostion = FVector2D::ZeroVector;

			FDrawSlot::FSlot InNewSlot = InPanels.Pop();
			FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop();
			float NestSpacing = HWith / 2.f + InNewSlot.GetSize().X / 2.f;
			if (i & 0x1)
			{
				L = FVector2D(L.X - NestSpacing, L.Y);
				InNewValidPostion = L;
			}
			else
			{
				R = FVector2D(R.X + NestSpacing, L.Y);
				InNewValidPostion = R;
			}
			InNewSlot.SetPostion(InNewValidPostion);

			FVector2D InNewItemPostion = FVector2D(InNewValidPostion.X, InNewValidPostion.Y + InItem.GetSize().Y);
			InItem.SetPostion(InNewItemPostion);

			RecursiveUpdateSlot(InItem.GetSlotTable(), InNewItemPostion, InSlotLayer -= 2);
		};

		//
		for (int32 i = 0; i < ValidNumebr; i++)
		{
			FVector2D InNewValidPostion = FVector2D::ZeroVector;

			if (DrawSlot.HLinePanel.Num())
			{
				FDrawSlot::FSlot InHor = DrawSlot.HLinePanel.Pop();
				float NestSpacing = InHor.GetSize().X / 2.f + InLRVLin.GetSize().X / 2.f;
				if (i & 0x1)
				{
					L = FVector2D(L.X - NestSpacing, L.Y);
					InNewValidPostion = L;
				}
				else
				{
					R = FVector2D(R.X + NestSpacing, L.Y);
					InNewValidPostion = R;
				}

				InHor.SetPostion(InNewValidPostion);
			}

			//
			if (DrawSlot.TLinePanel.Num())
			{
				Add(i, DrawSlot.TLinePanel, InSlotLayer);
			}
		}

		//
		for (int32 i = 0; i < 2; i++)
		{
			Add(i, DrawSlot.LREndPanel, InSlotLayer);
		}
	}
}
