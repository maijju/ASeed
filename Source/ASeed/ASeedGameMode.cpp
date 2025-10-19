// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGameMode.h"
#include "Player/ASeedPlayer.h"
#include "Player/ASeedPlayerController.h"

#include "UI/HUD/ASeedUI_LevelProgress.h"
#include "UI/ASeedUI_LevelUp.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"

#include "Effect/StatUp/ASeedGE_StatUpAmmoMax.h"
#include "Effect/StatUp/ASeedGE_StatUpAttack.h"
#include "Effect/StatUp/ASeedGE_StatUpAttackSpeed.h"
#include "Effect/StatUp/ASeedGE_StatUpCooldownReduce.h"
#include "Effect/StatUp/ASeedGE_StatUpDefense.h"
#include "Effect/StatUp/ASeedGE_StatUpHPMax.h"
#include "Effect/StatUp/ASeedGE_StatUpMoveSpeed.h"

#include "Data/ASeedGameData.h"

AASeedGameMode::AASeedGameMode()
{
    /*--------------DATA--------------*/
    WaveDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Game/DT_WaveData.DT_WaveData'"));
    LevelDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Game/DT_LevelData.DT_LevelData'"));
    LevelUpRewardDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Game/DT_LevelUpRewardData.DT_LevelUpRewardData'"));

    /*--------------SPAWN--------------*/
	SpawnOffsets.Reserve(NumOfSpawnOffsets);
}

void AASeedGameMode::BeginPlay()
{
    Super::BeginPlay();

    /*--------------LOAD DATA--------------*/
    WaveData = WaveDataRef.LoadSynchronous();
    FWaveData* WaveRow = WaveData->FindRow<FWaveData>(WaveKey, TEXT(""));
    if (!WaveRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *WaveKey.ToString());
        return;
    }
    Waves = WaveRow->Waves;

    LevelData = LevelDataRef.LoadSynchronous();
    FLevelData* LevelRow = LevelData->FindRow<FLevelData>(LevelKey, TEXT(""));
    if (!LevelRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *LevelKey.ToString());
        return;
    }
    Levels = LevelRow->Levels;

    LevelUpRewardData = LevelUpRewardDataRef.LoadSynchronous();
    FLevelUpRewardData* LevelUpRewardRow = LevelUpRewardData->FindRow<FLevelUpRewardData>(LevelUpRewardKey, TEXT(""));
    if (!LevelUpRewardRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *LevelKey.ToString());
        return;
    }
    LevelUpRewards = LevelUpRewardRow->LevelUpRewards;
    
    /*---------INITIALIZE SPAWN OFFSETS---------*/
    SpawnOffsets.Empty();
    const float DistanceFromPlayer = 2000.f;

    for (int32 i = 0; i < NumOfSpawnOffsets; ++i)
    {
        const float Angle = FMath::RandRange(0.f, 360.f);
        const float Radius = DistanceFromPlayer;

        const float X = FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius;
        const float Y = FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius;

        SpawnOffsets.Add(FVector(X, Y, 0.f));
    }

    /*---------SETUP---------*/
    Player = Cast<AASeedPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    ExecuteWave();
}

void AASeedGameMode::OnMainWidgetLoaded(UUserWidget* MainWidget)
{
    LevelHUD = Cast<UASeedUI_LevelProgress>(MainWidget->GetWidgetFromName(TEXT("LevelHUD")));
    LevelUpUI = Cast<UASeedUI_LevelUp>(MainWidget->GetWidgetFromName(TEXT("LevelUpUI")));

    if (LevelHUD)
    {
        LevelHUD->SetLevelLabel(CurrentLevel);
    }

    if (LevelUpUI)
    {
        LevelUpUI->SetVisibility(ESlateVisibility::Collapsed);
        LevelUpUI->OnCardSelected.AddDynamic(this, &AASeedGameMode::OnCardSelected);
    }
}

void AASeedGameMode::ExecuteWave()
{
    /*---------CHECK WAVE AVAILABILITY---------*/
    if (CurrentWave >= Waves.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("End of wave!"));
        return;
    }
    if (SpawnOffsets.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No spawn offsets available!"));
        return;
    }
    const int32 EnemyCount = Waves[CurrentWave].EnemyCount;
    if (EnemyCount > SpawnOffsets.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Trying to too much enemies!"));
        return;
    }

    /*---------CHOOSE SPAWN OFFSETS---------*/
    
    TArray<int32> Indices;
    Indices.Reserve(SpawnOffsets.Num());
    for (int32 i = 0; i < SpawnOffsets.Num(); ++i)
    {
        Indices.Add(i);
    }
    Indices.Sort([](int32 A, int32 B) { return FMath::RandBool(); });

    /*---------SPAWN ENEMIES---------*/
    for (int32 i = 0; i < EnemyCount; ++i)
    {
        const FVector SpawnLoc = Player->GetActorLocation() + SpawnOffsets[Indices[i]];
        const FRotator SpawnRot = FRotator::ZeroRotator;

        FActorSpawnParameters Param;
        Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AASeedEnemy* Enemy = GetWorld()->SpawnActor<AASeedEnemy>(SpawnLoc, SpawnRot, Param);
        if (Enemy)
        {
            Enemy->InitializeEnemy(Waves[CurrentWave].EnemyKey);
        }
    }

    /*---------RESERVE NEXT WAVE---------*/
    if (!Waves[CurrentWave].bIsBossWave)
    {
        GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AASeedGameMode::ExecuteWave, Waves[CurrentWave].TimeToCallNextWave, false);
    }
    CurrentWave++;
}

void AASeedGameMode::EarnEliminationRewards(const FRewards& Rewards)
{
    /*---------EARN CREDIT---------*/
    CurrentCredit += Rewards.CreditReward;

    /*---------CHECK EXP CONDITION---------*/
    if (CurrentLevel >= Levels.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Already at Max Level"));
        return;
    }

    /*---------EARN EXP---------*/
    CurrentExp += Rewards.ExpReward;

    /*---------LEVELUP---------*/
    while (CurrentLevel < Levels.Num() - 1 && CurrentExp >= Levels[CurrentLevel].MaxExp)
    {
        CurrentExp -= Levels[CurrentLevel].MaxExp;
        CurrentLevel++;
        LevelUp();
    }

    /*---------HANDLE MAX LEVEL---------*/
    if (CurrentLevel >= Levels.Num() - 1)
    {
        CurrentLevel = Levels.Num() - 1;
        CurrentExp = FMath::Min(CurrentExp, Levels[CurrentLevel].MaxExp);
    }

    /*---------UPDATE UI---------*/
    if (LevelHUD)
    {
        LevelHUD->UpdateExpPercent(CurrentExp, Levels[CurrentLevel].MaxExp);
    }

    UE_LOG(LogTemp, Warning, TEXT("Current Level: %d, Exp: %.1f / %.1f, Credit: %.1f"),
        CurrentLevel,
        CurrentExp,
        Levels[CurrentLevel].MaxExp,
        CurrentCredit);
}

void AASeedGameMode::LevelUp()
{
    /*---------PAUSE GAME IMMEDIATELY---------*/
    APlayerController* PC = Cast<APlayerController>(Player->GetController());
    PC->SetPause(true);
    
    /*---------SHUFFLE REWARDS---------*/
    TArray<FLevelUpRewardInfo> Result;
    TArray<int32> Indices;
    for (int32 i = 0; i < LevelUpRewards.Num(); i++)
    {
        Indices.Add(i);
    }
    for (int32 i = Indices.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        Indices.Swap(i, j);
    }
    for (int32 i = 0; i < 3; i++)
    {
        Result.Add(LevelUpRewards[Indices[i]]);
    }

    /*---------UPDATE UI---------*/
    LevelHUD->SetLevelLabel(CurrentLevel);
    LevelUpUI->InitializeCards(Result);
    LevelUpUI->SetVisibility(ESlateVisibility::Visible);
}

void AASeedGameMode::OnCardSelected(FCard Card)
{
    /*---------COLLAPSE LEVELUP UI---------*/
    LevelUpUI->SetVisibility(ESlateVisibility::Collapsed);
    
    /*---------RESTART GAME---------*/
    APlayerController* PC = Cast<APlayerController>(Player->GetController());
    PC->SetPause(false);

    /*---------APPLYING STATS---------*/
    UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
    if (!ASC) return;

    TSubclassOf<UGameplayEffect> EffectClass = nullptr;
    FName EffectTagName;
    float Value = (float)Card.Value;

    switch (Card.StatEnum)
    {
    case EPlayerStats::HPMax:
        EffectClass = UASeedGE_StatUpHPMax::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.HPMax");
        break;
    case EPlayerStats::AmmoMax:
        EffectClass = UASeedGE_StatUpAmmoMax::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.AmmoMax");
        break;
    case EPlayerStats::Attack:
        EffectClass = UASeedGE_StatUpAttack::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.Attack");
        break;
    case EPlayerStats::AttackSpeed:
        EffectClass = UASeedGE_StatUpAttackSpeed::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.AttackSpeed");
        Value /= 100;
        break;
    case EPlayerStats::MoveSpeed:
        EffectClass = UASeedGE_StatUpMoveSpeed::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.MoveSpeed");
        break;
    case EPlayerStats::Defense:
        EffectClass = UASeedGE_StatUpDefense::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.Defense");
        break;
    case EPlayerStats::CooldownReduce:
        EffectClass = UASeedGE_StatUpCooldownReduce::StaticClass();
        EffectTagName = FName("Custom.Effect.StatUp.CooldownReduce");
        Value /= 100;
        break;
    default:
        return;
    }

    if (!EffectClass) return;

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, ContextHandle);

    if (SpecHandle.IsValid())
    {
        SpecHandle.Data->SetSetByCallerMagnitude(
            FGameplayTag::RequestGameplayTag(EffectTagName),
            Value
        );

        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}