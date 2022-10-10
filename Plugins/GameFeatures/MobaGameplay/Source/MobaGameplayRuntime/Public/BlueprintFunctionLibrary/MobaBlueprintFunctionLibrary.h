// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "MobaBlueprintFunctionLibrary.generated.h"

class AAIController;
class ALyraGameMode;
class ALyraPlayerStart;
class ULyraHealthComponent;
class ULyraPawnExtensionComponent;
/**
 * C++蓝图函数库
 */
UCLASS()
class MOBAGAMEPLAYRUNTIME_API UMobaBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = LyraGameMode)
		static void CallDispatchPostLogin(AAIController* NewController, ALyraGameMode* GameMode);

	UFUNCTION(BlueprintCallable, Category = LyraPawnExtensionComponent)
		static void CallCheckPawnReadyToInitialize(ULyraPawnExtensionComponent* PawnExtComponent);

	UFUNCTION(BlueprintCallable, Category = LyraPawnExtensionComponent)
		static void CallSetPawnData(ULyraPawnExtensionComponent* PawnExtComponent, const ULyraPawnData* InPawnData);

	UFUNCTION(BlueprintCallable, Category = LyraHealthComponent)
		static void CallDamageSelfDestruct(ULyraHealthComponent* HealthComponent, bool bFellOutOfWorld = false);

	UFUNCTION(BlueprintCallable, Category = LyraPlayerStart)
		static const FGameplayTagContainer& CallGetGameplayTags(ALyraPlayerStart* PlayerStart);

	/** Did this player start get claimed by a controller already? */
	UFUNCTION(BlueprintCallable, Category = LyraPlayerStart)
		static bool CallIsClaimed(ALyraPlayerStart* PlayerStart);

	/** If this PlayerStart was not claimed, claim it for ClaimingController */
	UFUNCTION(BlueprintCallable, Category = LyraPlayerStart)
		static bool CallTryClaimed(ALyraPlayerStart* PlayerStart, AController* OccupyingController);

	// Changes the team associated with this actor if possible
	// Note: This function can only be called on the authority
	UFUNCTION(BlueprintCallable, Category = Teams)
		static bool ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex, const UWorld* InWorld);
};
