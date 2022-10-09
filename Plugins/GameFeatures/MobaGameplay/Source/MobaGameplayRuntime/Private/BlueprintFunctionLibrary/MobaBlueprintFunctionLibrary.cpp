// Copyright GanBowen. All Rights Reserved.
#include "BlueprintFunctionLibrary/MobaBlueprintFunctionLibrary.h"
#include "AIController.h"
#include "Player/LyraPlayerStart.h"
#include "GameModes/LyraGameMode.h"
#include "Character/LyraHealthComponent.h"
#include "Character/LyraPawnExtensionComponent.h"
#include "Teams/LyraTeamSubsystem.h"
#include "Engine/World.h"

void UMobaBlueprintFunctionLibrary::CallDispatchPostLogin(AAIController* NewController, ALyraGameMode* GameMode)
{
	GameMode->DispatchPostLogin(NewController);
}

void UMobaBlueprintFunctionLibrary::CallCheckPawnReadyToInitialize(ULyraPawnExtensionComponent* PawnExtComponent)
{
	PawnExtComponent->CheckPawnReadyToInitialize();
}

void UMobaBlueprintFunctionLibrary::CallSetPawnData(ULyraPawnExtensionComponent* PawnExtComponent, const ULyraPawnData* InPawnData)
{
	PawnExtComponent->SetPawnData(InPawnData);
}

void UMobaBlueprintFunctionLibrary::CallDamageSelfDestruct(ULyraHealthComponent* HealthComponent, bool bFellOutOfWorld /*= false*/)
{
	HealthComponent->DamageSelfDestruct(bFellOutOfWorld);
}

const FGameplayTagContainer& UMobaBlueprintFunctionLibrary::CallGetGameplayTags(ALyraPlayerStart* PlayerStart)
{
	return PlayerStart->GetGameplayTags();
}

bool UMobaBlueprintFunctionLibrary::CallIsClaimed(ALyraPlayerStart* PlayerStart)
{
	return PlayerStart->IsClaimed();
}

bool UMobaBlueprintFunctionLibrary::CallTryClaimed(ALyraPlayerStart* PlayerStart, AController* OccupyingController)
{
	return PlayerStart->TryClaim(OccupyingController);
}

bool UMobaBlueprintFunctionLibrary::ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex, const UWorld* InWorld)
{
	if (ULyraTeamSubsystem* TeamSubsystem = UWorld::GetSubsystem<ULyraTeamSubsystem>(InWorld))
	{
		if (TeamSubsystem->DoesTeamExist(NewTeamIndex))
		{
			return TeamSubsystem->ChangeTeamForActor(ActorToChange, NewTeamIndex);
		}
	}
	return false;
}
