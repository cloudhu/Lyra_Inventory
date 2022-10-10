// Copyright CloudHu. All Rights Reserved.
#include "GameModeExtension/SpawnerComponentBase.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/LyraExperienceDefinition.h"
#include "Development/LyraDeveloperSettings.h"
#include "Player/LyraPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Character/LyraPawnExtensionComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/LyraHealthComponent.h"
#include "Character/LyraPawnData.h"
#include "Player/LyraPlayerStart.h"
#include "EngineUtils.h"
#include "Teams/LyraTeamSubsystem.h"
//#include "GameModes/LyraGameMode.h"

USpawnerComponentBase::USpawnerComponentBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
	, NumToCreate(5)
	, TeamId(INDEX_NONE)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USpawnerComponentBase::BeginPlay()
{
	Super::BeginPlay();
	//找到所有的角色生成点,添加到缓存数组中
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ThisClass::OnLevelAdded);

	UWorld* World = GetWorld();
	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::HandleOnActorSpawned));

	for (TActorIterator<ALyraPlayerStart> It(World); It; ++It)
	{
		if (ALyraPlayerStart* PlayerStart = *It)
		{
			CachedPlayerStarts.Add(PlayerStart);
		}
	}
}

void USpawnerComponentBase::OnLevelAdded(ULevel* InLevel, UWorld* InWorld)
{
	if (InWorld == GetWorld())
	{
		for (AActor* Actor : InLevel->Actors)
		{
			if (ALyraPlayerStart* PlayerStart = Cast<ALyraPlayerStart>(Actor))
			{
				ensure(!CachedPlayerStarts.Contains(PlayerStart));
				CachedPlayerStarts.Add(PlayerStart);
			}
		}
	}
}

void USpawnerComponentBase::HandleOnActorSpawned(AActor* SpawnedActor)
{
	if (ALyraPlayerStart* PlayerStart = Cast<ALyraPlayerStart>(SpawnedActor))
	{
		CachedPlayerStarts.Add(PlayerStart);
	}
}

void USpawnerComponentBase::OnExperienceLoaded()
{
#if WITH_SERVER_CODE//服务器代码
	if (HasAuthority())
	{
		ServerCreateBots();
	}
#endif
}

void USpawnerComponentBase::SetTeamId(int32 NewId)
{
	TeamId = NewId;
}

#if WITH_SERVER_CODE
void USpawnerComponentBase::ServerCreateBots()
{
	if (ControllerClass == nullptr)
	{
		return;
	}

	// Create them
	for (int32 Count = 0; Count < NumToCreate; ++Count)
	{
		SpawnOneBot();
	}
}

void USpawnerComponentBase::SpawnOneBot()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetComponentLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;
	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(ControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

	if (NewController != nullptr)
	{
		//无法在GameFeature的插件中访问没有LYRAGAME_API修饰的LyraGame核心类,因此这部分逻辑转交给蓝图实现
		//ALyraGameMode* GameMode = GetGameMode<ALyraGameMode>();
		//check(GameMode);
		//GameMode->DispatchPostLogin(NewController);
		//GameMode->RestartPlayer(NewController);
		LyraGameModeDispatchPostLogin(NewController);
		//如果有指定队伍,这里会改变队伍;如果没有指定,就使用自动分配的队伍
		ChangeTeam(NewController);
		SpawnCharacter(NewController);

		if (NewController->GetPawn() != nullptr)
		{
			if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
			{
				PawnExtComponent->CheckPawnReadyToInitialize();
			}
		}

		SpawnedBotList.Add(NewController);
	}
}

void USpawnerComponentBase::SpawnCharacter(AAIController* NewController)
{
	if (NewController == nullptr || NewController->IsPendingKillPending())
	{
		return;
	}
	FRotator StartRotation(ForceInit);

	if (NewController->GetPawn() != nullptr)
	{
		StartRotation = NewController->GetPawn()->GetActorRotation();
	}
	else if (PawnData->PawnClass != nullptr)
	{
		if (ALyraPlayerStart* LyraStart = ChoosePlayerStart(NewController))
		{

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Instigator = nullptr;
			SpawnInfo.ObjectFlags |= RF_Transient;
			SpawnInfo.bDeferConstruction = true;

			StartRotation.Yaw = LyraStart->GetActorRotation().Yaw;
			FVector StartLocation = LyraStart->GetActorLocation();
			FTransform SpawnTransform = FTransform(StartRotation, StartLocation);

			if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnData->PawnClass, SpawnTransform, SpawnInfo))
			{
				if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				NewController->SetPawn(SpawnedPawn);
				LyraStart->TryClaim(NewController);
				SpawnedPawn->FinishSpawning(SpawnTransform);
				if (!IsValid(NewController->GetPawn()))
				{
					NewController->FailedToSpawnPawn();
				}
				else
				{
					NewController->Possess(NewController->GetPawn());
					if (!IsValid(NewController->GetPawn()))
					{
						NewController->FailedToSpawnPawn();
					}
					else
					{
						NewController->ClientSetRotation(NewController->GetPawn()->GetActorRotation(), true);

						FRotator NewControllerRot = StartRotation;
						NewControllerRot.Roll = 0.f;
						NewController->SetControlRotation(NewControllerRot);

						SpawnedPawn->SetPlayerDefaults();
					}
				}

				PostLyraASC(SpawnedPawn, LyraStart->GetGameplayTags());
			}
		}
	}

}

void USpawnerComponentBase::ChangeTeam(AAIController* NewController)
{
	if (TeamId != INDEX_NONE)
	{
		if (ULyraTeamSubsystem* TeamSubsystem = UWorld::GetSubsystem<ULyraTeamSubsystem>(GetWorld()))
		{
			const int32 CurretTeamId = TeamSubsystem->FindTeamFromObject(NewController);
			if (TeamId != CurretTeamId)
			{
				TeamSubsystem->ChangeTeamForActor(NewController, TeamId);
			}
		}
	}
}

void USpawnerComponentBase::RemoveOneBot()
{
	if (SpawnedBotList.Num() > 0)
	{
		// Right now this removes a random bot as they're all the same; could prefer to remove one
		// that's high skill or low skill or etc... depending on why you are removing one
		const int32 BotToRemoveIndex = FMath::RandRange(0, SpawnedBotList.Num() - 1);

		AAIController* BotToRemove = SpawnedBotList[BotToRemoveIndex];
		SpawnedBotList.RemoveAtSwap(BotToRemoveIndex);

		if (BotToRemove)
		{
			// If we can find a health component, self-destruct it, otherwise just destroy the actor
			if (APawn* ControlledPawn = BotToRemove->GetPawn())
			{
				if (ULyraHealthComponent* HealthComponent = ULyraHealthComponent::FindHealthComponent(ControlledPawn))
				{
					// Note, right now this doesn't work quite as desired: as soon as the player state goes away when
					// the controller is destroyed, the abilities like the death animation will be interrupted immediately
					HealthComponent->DamageSelfDestruct();
				}
				else
				{
					ControlledPawn->Destroy();
				}
			}

			// Destroy the controller (will cause it to Logout, etc...)
			BotToRemove->Destroy();
		}
	}
}

ALyraPlayerStart* USpawnerComponentBase::ChoosePlayerStart(AController* NewController)
{
	if (ULyraTeamSubsystem* TeamSubsystem = UWorld::GetSubsystem<ULyraTeamSubsystem>(GetWorld()))
	{
		const int32 CurretTeamId = TeamSubsystem->FindTeamFromObject(NewController);
		FGameplayTag TeamTag;
		if (CurretTeamId == 1)
		{
			TeamTag = RedTeam;
		}
		else if (CurretTeamId == 2)
		{
			TeamTag = BlueTeam;
		}

		for (auto StartIt = CachedPlayerStarts.CreateIterator(); StartIt; ++StartIt)
		{
			if (ALyraPlayerStart* playerStart = (*StartIt).Get())
			{
				if (!playerStart->IsClaimed())
				{
					const FGameplayTagContainer& StartTags = playerStart->GetGameplayTags();
					if (StartTags.HasTagExact(TeamTag))
					{
						if (StartTags.HasAllExact(CharacterTags))
						{
							return playerStart;
						}
					}
				}
			}

		}
	}
	return nullptr;
}

#endif