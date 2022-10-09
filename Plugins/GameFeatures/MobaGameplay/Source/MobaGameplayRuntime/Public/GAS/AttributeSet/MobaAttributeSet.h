// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MobaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_MULTICAST_DELEGATE_FourParams(FLyraAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * 用于Moba玩法的属性集
 */
UCLASS(BlueprintType)
class MOBAGAMEPLAYRUNTIME_API UMobaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMobaAttributeSet();

	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Soul);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, HealthResume);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, ManaResume);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, SoulResume);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, CriticalChance);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MagicPower);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MagicResistance);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, PhysicalAttack);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, HealthSteal);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MagicStealHealth);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, SkillAccelerate);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, CriticalDamage);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, TrueDamage);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, ArmorPenetration);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MagicPenetration);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, ShieldPower);
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, AttackRange);

	UWorld* GetWorld() const override;

protected:

	UFUNCTION()
		void OnRep_Soul(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_HealthResume(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_ManaResume(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_SoulResume(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_CriticalChance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_MagicPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_MagicResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_Armor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_PhysicalAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_HealthSteal(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_MagicStealHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_SkillAccelerate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_CriticalDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_TrueDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_MagicPenetration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_ShieldPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		void OnRep_AttackRange(const FGameplayAttributeData& OldValue);
private:

	// The current Soul attribute. 游戏内基础货币
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Soul, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魂力")
		FGameplayAttributeData Soul;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魔法值")
		FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "最大魔法值")
		FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthResume, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "基础治疗/5s")
		FGameplayAttributeData HealthResume;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaResume, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "基础魔法值回复/5s")
		FGameplayAttributeData ManaResume;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SoulResume, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魂力回复/5s")
		FGameplayAttributeData SoulResume;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "暴击几率")
		FGameplayAttributeData CriticalChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicPower, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "法术强度")
		FGameplayAttributeData MagicPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "护甲")
		FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistance, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魔法抗性")
		FGameplayAttributeData MagicResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalAttack, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "物理攻击力")
		FGameplayAttributeData PhysicalAttack;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "攻击速度")
		FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthSteal, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "生命偷取")
		FGameplayAttributeData HealthSteal;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicStealHealth, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魔法吸血")
		FGameplayAttributeData MagicStealHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SkillAccelerate, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "技能急速")
		FGameplayAttributeData SkillAccelerate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "暴击伤害")
		FGameplayAttributeData CriticalDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TrueDamage, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "真实伤害")
		FGameplayAttributeData TrueDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "护甲穿透")
		FGameplayAttributeData ArmorPenetration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicPenetration, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "魔法穿透")
		FGameplayAttributeData MagicPenetration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldPower, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "能量护盾值")
		FGameplayAttributeData ShieldPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackRange, Category = "Moba", Meta = (AllowPrivateAccess = true), DisplayName = "攻击范围")
		FGameplayAttributeData AttackRange;
};
