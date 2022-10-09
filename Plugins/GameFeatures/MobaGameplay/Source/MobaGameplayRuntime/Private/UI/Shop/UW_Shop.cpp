// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_Shop.h"
#include "UI/Shop/Item/UW_ItemCompound.h"
#include "UI/Shop/ShopPlayerStart.h"
#include "Teams/LyraTeamSubsystem.h"
#include "Components/CheckBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/UniformGridSlot.h"
#include "ThreadManage.h"
#include "UI/Shop/UW_Inventory.h"
#include "GAS/AttributeSet/MobaAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UUW_Shop::UUW_Shop(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{//构造函数初始化默认值
	Time = 0.f;
	ShopLocation = FVector::ZeroVector;
}

void UUW_Shop::NativeConstruct()
{
	Super::NativeConstruct();
	UWorld* World = GetWorld();

	//1.寻找商店的出生点,从而记录商店位置
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ULyraTeamSubsystem* TeamSubsystem = UWorld::GetSubsystem<ULyraTeamSubsystem>(World))
		{//通过队伍子系统来判断玩家所属队伍
			const int32 CurretTeamId = TeamSubsystem->FindTeamFromObject(PC);
			FGameplayTag TeamTag = FGameplayTag::RequestGameplayTag(FName("CharacterType.Red"));
			if (CurretTeamId == 2)
			{
				TeamTag = FGameplayTag::RequestGameplayTag(FName("CharacterType.Blue"));
			}
			//循环寻找商店的出生点,找到之后获取其位置
			for (TActorIterator<AShopPlayerStart> It(World); It; ++It)
			{
				if (AShopPlayerStart* PlayerStart = *It)
				{
					if (PlayerStart->GetGameplayTags().HasTagExact(TeamTag))
					{
						ShopLocation = PlayerStart->GetNavAgentLocation();
					}
				}
			}
		}
	}
	//2.将所有勾选框添加到数组中,进行统一管理
	CheckBoxArray.Add(All);
	CheckBoxArray.Add(Consumables);
	CheckBoxArray.Add(AuxiliaryProps);
	CheckBoxArray.Add(PhysicalAttack);
	CheckBoxArray.Add(MagicAttack);
	CheckBoxArray.Add(PhysicalPenetration);
	CheckBoxArray.Add(MagicPenetration);
	CheckBoxArray.Add(Crit);
	CheckBoxArray.Add(Armor);
	CheckBoxArray.Add(MagicResistance);
	CheckBoxArray.Add(AttackSpeed);
	CheckBoxArray.Add(Shoes);

	//3.绑定勾选框的状态变化事件到对应的处理函数
	All->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxAll);
	Consumables->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxConsumables);
	AuxiliaryProps->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxAuxiliaryProps);
	PhysicalAttack->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxPhysicalAttack);
	MagicAttack->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxMagicAttack);
	PhysicalPenetration->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxPhysicalPenetration);
	MagicPenetration->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxMagicPenetration);
	Crit->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxCrit);
	Armor->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxArmor);
	MagicResistance->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxMagicResistance);
	AttackSpeed->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxAttackSpeed);
	Shoes->OnCheckStateChanged.AddDynamic(this, &UUW_Shop::CheckBoxShoes);

	//默认隐藏商店
	SetVisibility(ESlateVisibility::Hidden);
	//4.绑定魂力变化事件
	BindSoulNumChanged();
}

void UUW_Shop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Time += InDeltaTime;
	if (Time >= 0.3f)
	{
		Time = 0.f;
		if (IsScopeStore())
		{

		}
	}
}

void UUW_Shop::BindSoulNumChanged()
{
	if (APlayerState* PS = GetPlayerState())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PS))
		{
			// 注册属性变化代理函数,监听属性的变化
			ASC->GetGameplayAttributeValueChangeDelegate(UMobaAttributeSet::GetSoulAttribute()).AddUObject(this, &ThisClass::HandleSoulNumChanged);
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
				{
					BindSoulNumChanged();
				});
		}
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
			{
				BindSoulNumChanged();
			});
	}
}

void UUW_Shop::HandleSoulNumChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateAll();
}

void UUW_Shop::UpdateItem(ESlotType Type)
{
	ItemGrid->ClearChildren();
	//1.获取物品数据
	if (const TArray<FSlotTable*>* InSlotTableArray = GetSlotTablesTemplate())
	{
		TArray<const FSlotTable*> InTableArray;
		for (const auto& Tmp : *InSlotTableArray)
		{
			if (Tmp->SlotType.Contains(Type))
			{
				InTableArray.Add(Tmp);
			}
		}
		//2.循环遍历创建商品
		for (int32 i = 0; i < InTableArray.Num(); i++)
		{
			if (UUW_Item* InSlotWidget = CreateWidget<UUW_Item>(GetWorld(), ItemClass))
			{
				if (UUniformGridSlot* GridSlot = ItemGrid->AddChildToUniformGrid(InSlotWidget))
				{
					//i = 0 , 1  2 3  ,4,5
					//(0,0) (0,1)
					//(1,0) (1,1)
					//(2,0) (2,1)
					GridSlot->SetRow(FMath::FloorToInt((float)i / 2.f));
					if (i & 0x1)
					{
						GridSlot->SetColumn(1);
					}
					else
					{
						GridSlot->SetColumn(0);
					}

					GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

					InSlotWidget->UpdateSlot(InTableArray[i]);
					//绑定合成界面更新代理
					InSlotWidget->CallEquipmentSynthesisDelegate.BindUObject(this, &UUW_Shop::OnCallUpdateEquipmentCombination);
				}
			}
		}
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.3f, [&]()
			{
				UpdateItem(ESlotType::SLOT_ALL);
			});
	}

}

void UUW_Shop::UpdateAll()
{
	//遍历商品,根据玩家魂力激活商品
	for (auto& Tmp : ItemGrid->GetAllChildren())
	{
		if (UUW_Item* InItem = Cast<UUW_Item>(Tmp))
		{
			int32 soulNum = InItem->GetItemSoulNum();
			InItem->SetItemSoul(soulNum);
			if (GetPlayerSoulNum() < soulNum)
			{
				InItem->Enabled(false);
			}
			else
			{
				InItem->Enabled(true);				
			}
		}
	}
	//更新合成面板
	if (EquipmentCombination)
	{
		EquipmentCombination->UpdateAllItem();
	}
}


void UUW_Shop::OnCloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (ItemGrid)
	{
		ItemGrid->ClearChildren();
	}

	OnCloseWidgetComplete();
}

void UUW_Shop::OnNativeKey()
{
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);

		UpdateItem(ESlotType::SLOT_ALL);
	}
	else
	{
		OnCloseWidget();
	}
}

void UUW_Shop::OnCallUpdateEquipmentCombination(int32 InItemSlotID)
{
	if (const FSlotTable* InSlotTable = GetSlotTableTemplate(InItemSlotID))
	{
		EquipmentCombination->UpdateSlot(InSlotTable);
	}
}

//下面都是勾选框的操作
void UUW_Shop::CheckBoxAll(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	All->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_ALL);
}

void UUW_Shop::CheckBoxConsumables(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Consumables->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_CONSUMABLES);
}

void UUW_Shop::CheckBoxAuxiliaryProps(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	AuxiliaryProps->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_AUXILIARYPROPS);
}

void UUW_Shop::CheckBoxPhysicalAttack(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	PhysicalAttack->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_PHYSICALATTACK);
}

void UUW_Shop::CheckBoxMagicAttack(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicAttack->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_MAGICATTACK);
}

void UUW_Shop::CheckBoxPhysicalPenetration(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	PhysicalPenetration->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_PHYSICALPENETRATION);
}

void UUW_Shop::CheckBoxMagicPenetration(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicPenetration->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_MAGICPENETRATION);
}

void UUW_Shop::CheckBoxCrit(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Crit->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_CRIT);
}

void UUW_Shop::CheckBoxArmor(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Armor->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_ARMOR);
}

void UUW_Shop::CheckBoxMagicResistance(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicResistance->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_MAGICRESISTANCE);
}

void UUW_Shop::CheckBoxAttackSpeed(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	AttackSpeed->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_ATTACKSPEED);
}

void UUW_Shop::CheckBoxShoes(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Shoes->SetCheckedState(ECheckBoxState::Checked);
	UpdateItem(ESlotType::SLOT_SHOES);
}

void UUW_Shop::SetCheckBoxArray(ECheckBoxState CheckBoxState)
{
	for (auto& Tmp : CheckBoxArray)
	{
		Tmp->SetCheckedState(CheckBoxState);
	}
}

bool UUW_Shop::IsScopeStore()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			//为了避免开平方消耗性能,这里使用DistSquared
			float Distance = FVector::DistSquared(ShopLocation, Pawn->GetActorLocation());
			if (Distance < 1600000.f)
			{
				return true;
			}
			else
			{
				OnCloseWidget();
			}
		}
	}
	return false;
}

FReply UUW_Shop::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (HasMouseCapture())
	{
		if (UWidgetTree* InWidgetTree = Cast<UWidgetTree>(GetOuter()))
		{
			if (UWidget* InParentWidget = Cast<UWidget>(InWidgetTree->GetOuter()))
			{
				const FGeometry& InParentGeometry = InParentWidget->GetTickSpaceGeometry();
				const FVector2D& NewPostion = InMouseEvent.GetScreenSpacePosition();

				FVector2D NewOffsetA = InParentGeometry.AbsoluteToLocal(NewPostion) - Offset;
				FVector2D NewOffsetB = InParentGeometry.GetLocalSize() - GetDesiredSize();
				UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(Slot);
				if (NewPanelSlot)
				{
					NewPanelSlot->SetPosition(FVector2D(FMath::Clamp(NewOffsetA.X, 0.f, NewOffsetB.X), FMath::Clamp(NewOffsetA.Y, 0.f, NewOffsetB.Y)));
					Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
				}
			}
		}
	}

	return Reply;
}

FReply UUW_Shop::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		if (Offset.Y <= 80.f)
		{
			Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
		}
		else
		{
			Reply.ReleaseMouseCapture();
		}
	}

	return Reply;
}

FReply UUW_Shop::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	Reply.ReleaseMouseCapture();
	return Reply;
}
