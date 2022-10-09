// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Player/LyraPlayerSpawningManagerComponent.h"
#include "TwoTeamSpawningManagerComponent.generated.h"

/**
 * 两个队伍角色生成管理器组件
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UTwoTeamSpawningManagerComponent : public ULyraPlayerSpawningManagerComponent
{
	GENERATED_BODY()
public:

	UTwoTeamSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<ALyraPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;
protected:
	//红队
	const FGameplayTag& RedTeam = FGameplayTag::RequestGameplayTag(FName("CharacterType.Red"));
	//蓝队
	const FGameplayTag& BlueTeam = FGameplayTag::RequestGameplayTag(FName("CharacterType.Blue"));
	const FGameplayTag& PlayerTag = FGameplayTag::RequestGameplayTag(FName("CharacterType.Player"));
};