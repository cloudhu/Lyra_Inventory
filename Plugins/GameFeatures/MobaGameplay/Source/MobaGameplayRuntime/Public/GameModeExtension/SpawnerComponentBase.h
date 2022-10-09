// Copyright GanBowen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "SpawnerComponentBase.generated.h"

class ULyraExperienceDefinition;
class ULyraPawnData;
class AAIController;
class ALyraPlayerStart;
/**
 * 生成器组件基类
 */
UCLASS(Blueprintable, Abstract)
class MOBAGAMEPLAYRUNTIME_API USpawnerComponentBase : public UGameStateComponent
{
	GENERATED_BODY()
public:
	USpawnerComponentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

	UFUNCTION(BlueprintCallable, Category = Spawner)
		void OnExperienceLoaded();

	//通过蓝图来完成登录后处理
	UFUNCTION(BlueprintImplementableEvent, Category = Spawner)
		void LyraGameModeDispatchPostLogin(AAIController* NewController);

	//通过蓝图处理ASC的标签和技能
	UFUNCTION(BlueprintImplementableEvent, Category = Spawner)
		void PostLyraASC(APawn* SpawnedPawn, const FGameplayTagContainer& StartTags);

	UFUNCTION(BlueprintCallable, Category = Spawner)
		void SetTeamId(int32 NewId);

	UFUNCTION(BlueprintCallable, Category = Spawner)
		int32 GetTeamId() const { return TeamId; }

	UFUNCTION(BlueprintCallable, Category = Spawner)
		const	FGameplayTagContainer& GetCharacterTags() const { return CharacterTags; }
protected:
	/** 生成角色数量*/
	UPROPERTY(EditDefaultsOnly, Category = SpawnerConfig)
		int32 NumToCreate;

	UPROPERTY(EditAnywhere, Category = SpawnerConfig)
		int32 TeamId;

	/** 控制器类*/
	UPROPERTY(EditDefaultsOnly, Category = SpawnerConfig)
		TSubclassOf<AAIController> ControllerClass;

	/** 角色数据*/
	UPROPERTY(EditDefaultsOnly, Category = SpawnerConfig)
		TObjectPtr<const ULyraPawnData> PawnData;

	/** Tags to identify this player Character 角色标签*/
	UPROPERTY(EditAnywhere, Category = SpawnerConfig)
		FGameplayTagContainer CharacterTags;

protected:
	UPROPERTY(Transient)
		TArray<TObjectPtr<AAIController>> SpawnedBotList;

	UPROPERTY(Transient)
		TArray<TWeakObjectPtr<ALyraPlayerStart>> CachedPlayerStarts;

	//红蓝双方的标签
	const FGameplayTag& RedTeam = FGameplayTag::RequestGameplayTag(FName("CharacterType.Red"));
	const FGameplayTag& BlueTeam = FGameplayTag::RequestGameplayTag(FName("CharacterType.Blue"));

private:
	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	void HandleOnActorSpawned(AActor* SpawnedActor);

#if WITH_SERVER_CODE
public:
	void Cheat_AddBot() { SpawnOneBot(); }
	void Cheat_RemoveBot() { RemoveOneBot(); }

protected:
	virtual void ServerCreateBots();

	virtual void SpawnOneBot();

	virtual void SpawnCharacter(AAIController* NewController);

	virtual void ChangeTeam(AAIController* NewController);

	virtual void RemoveOneBot();

	virtual ALyraPlayerStart* ChoosePlayerStart(AController* NewController);
#endif
};
