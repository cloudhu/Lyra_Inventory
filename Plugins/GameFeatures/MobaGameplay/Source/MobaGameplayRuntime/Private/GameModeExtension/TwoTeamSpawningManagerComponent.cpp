// Copyright GanBowen. All Rights Reserved.
#include "GameModeExtension/TwoTeamSpawningManagerComponent.h"
#include "Teams/LyraTeamSubsystem.h"
#include "Player/LyraPlayerStart.h"
#include "Engine/World.h"

UTwoTeamSpawningManagerComponent::UTwoTeamSpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

AActor* UTwoTeamSpawningManagerComponent::OnChoosePlayerStart(AController* Player, TArray<ALyraPlayerStart*>& PlayerStarts)
{
	ULyraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<ULyraTeamSubsystem>();
	const int32 PlayerTeamId = TeamSubsystem->FindTeamFromObject(Player);

	// We should have a TeamId by now, but early login stuff before post login can try to do stuff, ignore it.
	if (!ensure(PlayerTeamId != INDEX_NONE))
	{
		return nullptr;
	}

	ALyraPlayerStart* FallbackPlayerStart = nullptr;
	FGameplayTag TeamTag;
	if (PlayerTeamId == 1)
	{
		TeamTag = RedTeam;
	}

	if (PlayerTeamId == 2)
	{
		TeamTag = BlueTeam;
	}

	for (ALyraPlayerStart* PlayerStart : PlayerStarts)
	{
		if (PlayerStart->GetGameplayTags().HasTagExact(PlayerTag) && PlayerStart->GetGameplayTags().HasTagExact(TeamTag))
		{
			if (PlayerStart->IsClaimed())
			{
				if (FallbackPlayerStart == nullptr)
				{
					FallbackPlayerStart = PlayerStart;
				}
			}
			else if (PlayerStart->GetLocationOccupancy(Player) < ELyraPlayerStartLocationOccupancy::Full)
			{
				return PlayerStart;
			}
		}
	}

	return FallbackPlayerStart;
}

void UTwoTeamSpawningManagerComponent::OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation)
{

}
