// Copyright GanBowen. All Rights Reserved.
#include "GAS/AttributeSet/MobaAttributeSet.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"

UMobaAttributeSet::UMobaAttributeSet()
	:Soul(INT32_MAX)
	, Mana(300.f)
	, MaxMana(300.f)
	, HealthResume(5.2f)
	, ManaResume(8.1f)
	, SoulResume(5.1f)
	, CriticalChance(0.01f)
	, MagicPower(0.f)
	, Armor(30.f)
	, MagicResistance(34.f)
	, PhysicalAttack(79.f)
	, AttackSpeed(0.72f)
	, HealthSteal(0.f)
	, MagicStealHealth(0.f)
	, SkillAccelerate(0.f)
	, CriticalDamage(1.5f)
	, TrueDamage(0.f)
	, ArmorPenetration(0.f)
	, MagicPenetration(0.f)
	, ShieldPower(0.f)
	, AttackRange(125.f)
{
}

UWorld* UMobaAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

void UMobaAttributeSet::OnRep_Soul(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, Soul, OldValue);
}

void UMobaAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, Mana, OldValue);
}

void UMobaAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, MaxMana, OldValue);
}

void UMobaAttributeSet::OnRep_HealthResume(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, HealthResume, OldValue);
}

void UMobaAttributeSet::OnRep_ManaResume(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, ManaResume, OldValue);
}

void UMobaAttributeSet::OnRep_SoulResume(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, SoulResume, OldValue);
}

void UMobaAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, CriticalChance, OldValue);
}

void UMobaAttributeSet::OnRep_MagicPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, MagicPower, OldValue);
}

void UMobaAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, MagicResistance, OldValue);
}

void UMobaAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, Armor, OldValue);
}

void UMobaAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, PhysicalAttack, OldValue);
}

void UMobaAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, AttackSpeed, OldValue);
}

void UMobaAttributeSet::OnRep_HealthSteal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, HealthSteal, OldValue);
}

void UMobaAttributeSet::OnRep_MagicStealHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, MagicStealHealth, OldValue);
}

void UMobaAttributeSet::OnRep_SkillAccelerate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, SkillAccelerate, OldValue);
}

void UMobaAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, CriticalDamage, OldValue);
}

void UMobaAttributeSet::OnRep_TrueDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, TrueDamage, OldValue);
}

void UMobaAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, ArmorPenetration, OldValue);
}

void UMobaAttributeSet::OnRep_MagicPenetration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, MagicPenetration, OldValue);
}

void UMobaAttributeSet::OnRep_ShieldPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, ShieldPower, OldValue);
}

void UMobaAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMobaAttributeSet, AttackRange, OldValue);
}

void UMobaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, Soul, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, HealthResume, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, ManaResume, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, SoulResume, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, HealthSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, MagicStealHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, SkillAccelerate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, TrueDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, MagicPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, ShieldPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMobaAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
}