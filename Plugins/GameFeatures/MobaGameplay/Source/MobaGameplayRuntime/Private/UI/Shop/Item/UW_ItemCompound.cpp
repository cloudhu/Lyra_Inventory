// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/Item/UW_ItemCompound.h"
#include "UI/Shop/Item/UW_ItemSlot.h"
#include "PlayerStateExtension/MobaPlayerStateComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
//绘制合成装备的最小单元
struct FDrawSlot
{
	//绘制画布里各种元素的坐标,主要是做线段的绘制
	struct FSlot
	{
		FSlot(UCanvasPanelSlot* InSlot)
			:Slot(InSlot)
		{}
		//设置槽位所在画布上的位置
		void SetPostion(const FVector2D& InPostion);
		//获取其尺寸
		FVector2D GetSize() const;
	private:
		UCanvasPanelSlot* Slot;//画布上的槽位
	};
	//绘制装备槽,点击时可以购买装备
	struct FTable :public FSlot
	{
		FTable(UCanvasPanelSlot* InSlot, const FSlotTable* InTableSlot)
			:FSlot(InSlot)
			, TableSlot(InTableSlot)
		{}

		const FSlotTable* GetSlotTable();
	private:
		const FSlotTable* TableSlot;//缓存装备数据
	};
	//类似乐高积木,这里通过数组来记录所有合成所需的积木素材,然后再搭建最终的作品
	TArray<FSlot> LREndPanel;//两端的拐弯 ⌈ 或 ⌉
	TArray<FSlot> VLinePanel;//|
	TArray<FSlot> HLinePanel;//_
	TArray<FSlot> TLinePanel;//T
	TArray<FSlot> LRVPanel;	//_|_ 
	TArray<FTable> ItemPanel;//装备按钮
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
	//0.清除上一个物品的合成图
	EquipmentPanel->ClearChildren();

	if (InTable)
	{
		//1.根据数据表递归获取子类的深度,根据深度来盖楼
		int32 InLayer = GetLayerDepth(InTable);
		//2.创建物品槽来显示物品
		if (UUW_ItemSlot* InMainSlotWidget = CreateWidget<UUW_ItemSlot>(GetWorld(), ItemSlotClass))
		{
			//3.将生成的物品槽添加到面板中,设置其位置,递归更新子类
			if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InMainSlotWidget))
			{
				InPanelSlot->SetSize(IconSize);
				InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));
				InPanelSlot->SetPosition(StartPostion);

				RecursiveUpdateSlot(InTable, StartPostion, --InLayer);
			}
			//4.根据数据更新物品信息
			InMainSlotWidget->UpdateSlot(InTable);
			//5.检查玩家魂力是否足够激活物品
			CheckSoul(InTable, InMainSlotWidget);
		}
	}
}

void UUW_ItemCompound::UpdateAllItem()
{
	//更新合成面板的所有子类,判断玩家魂力是否足够激活物品
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
	//递归物品数据中的子类数组,获得楼层
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
	//0.合成物品所需的数量,这里渲染整个合成图谱的思路就是先生成合成所需的素材,然后再来布局.这种思路就像是拼图一样,先有各种碎片,再根据碎片来拼凑完整的图像
	int32 NumberChildrens = InTable->Childrens.Num();
	//1.画格子的Lambda函数,关于Lambda函数大家可以百度去了解一下,这里的复杂度已经很高了,所以不做科普了
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
	//2.生成物品Lambda函数
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

	//3.生成所有所需的素材
//////////////////////////////////////////////////////////////////////////
	if (NumberChildrens == 1)//仅有一个子类,画一条垂直线即可
	{
		SpawnSlot(DrawSlot.VLinePanel, LineTexture.VerticalLine);
	}
	else if (NumberChildrens >= 2)//两个以上子类,需要拆分
	{
		//生成 ⌉ 型 线条
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnRight);
		//生成 ⌈ 型 线条
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnLeft);
		//生成 _|_ 型线
		SpawnSlot(DrawSlot.LRVPanel, LineTexture.TurnLeftAndRightVertically);

		//生成 T 型线
		for (int32 i = 0; i < NumberChildrens - 2; i++)
		{
			SpawnSlot(DrawSlot.TLinePanel, LineTexture.TLine);
		}
		//生成 - 水平线
		for (int32 i = 0; i < NumberChildrens; i++)
		{
			SpawnSlot(DrawSlot.HLinePanel, LineTexture.HorizontalLine, InSlotLayer);
		}
	}

	//在格子中生成物品
	for (auto& Tmp : InTable->Childrens)
	{
		if (const FSlotTable* InSlotTable = GetSlotTableTemplate(Tmp))
		{
			SpawnItem(DrawSlot.ItemPanel, InSlotTable);
		}
	}
	//收集所有的线条有效数量
	int32 ValidNumebr = DrawSlot.HLinePanel.Num() +
		DrawSlot.LREndPanel.Num() +
		DrawSlot.LRVPanel.Num() +
		DrawSlot.TLinePanel.Num() +
		DrawSlot.VLinePanel.Num();

	//数量是1的情况,通过垂直线连接父子,类似 吕 字型布局
	if (NumberChildrens == 1)
	{
		FDrawSlot::FSlot InVLin = DrawSlot.VLinePanel.Pop();//垂直线
		FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop();//子物品
		FVector2D NewPos = FVector2D(InStartPostion.X, InStartPostion.Y + InVLin.GetSize().Y);//计算垂直线位置:从起始位置往下移动垂直线Y轴长度
		InVLin.SetPostion(NewPos);//设置垂直线在父物品的下方
		NewPos = FVector2D(NewPos.X, NewPos.Y + InVLin.GetSize().Y);//计算子物品的位置
		InItem.SetPostion(NewPos);//将子物品设置在垂直线下方,最终形成 吕 字型布局,用垂直线连接父和子物品
	}
	else if (NumberChildrens >= 2)
	{
		FDrawSlot::FSlot InLRVLin = DrawSlot.LRVPanel.Pop();//先获得一个生成 _|_ 型线来连接父物品
		FVector2D NewPos = FVector2D(InStartPostion.X, InStartPostion.Y + InLRVLin.GetSize().Y);//从起始位置往下移动线段的Y轴长度
		InLRVLin.SetPostion(NewPos);//布局位置

		float HWith = DrawSlot.HLinePanel[0].GetSize().X;//获取水平线宽度
		FVector2D R = NewPos;//声明右侧的位置
		FVector2D L = NewPos;//声明左侧的位置
		//递归子项,计算拐点
		auto Add = [&](int32 i, TArray<FDrawSlot::FSlot>& InPanels, FSlotLayer InSlotLayer)
		{
			FVector2D InNewValidPostion = FVector2D::ZeroVector;//声明新的有效位置

			FDrawSlot::FSlot InNewSlot = InPanels.Pop();//弹出基本元素,可以想象成拼图所用的碎片
			FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop();//物品
			float NextSpacing = HWith / 2.f + InNewSlot.GetSize().X / 2.f;//计算下一个相邻物品的间隔
			if (i & 0x1)//奇数:代表左侧
			{
				L = FVector2D(L.X - NextSpacing, L.Y);//左侧位置用X轴减间隔,这个是笛卡尔坐标系基本常识
				InNewValidPostion = L;//得到新的有效坐标
			}
			else//偶数:代表右侧
			{
				R = FVector2D(R.X + NextSpacing, L.Y);//右侧位置用X轴加间隔
				InNewValidPostion = R;
			}
			InNewSlot.SetPostion(InNewValidPostion);//设置格子的位置

			FVector2D InNewItemPostion = FVector2D(InNewValidPostion.X, InNewValidPostion.Y + InItem.GetSize().Y);//计算物品位置
			InItem.SetPostion(InNewItemPostion);//设置物品位置
			//递归更新物品
			RecursiveUpdateSlot(InItem.GetSlotTable(), InNewItemPostion, InSlotLayer -= 2);
		};

		//绘制水平线段
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

			//绘制 T 型线段
			if (DrawSlot.TLinePanel.Num())
			{
				Add(i, DrawSlot.TLinePanel, InSlotLayer);
			}
		}

		//绘制左右拐点线段
		for (int32 i = 0; i < 2; i++)
		{
			Add(i, DrawSlot.LREndPanel, InSlotLayer);
		}
	}
}
