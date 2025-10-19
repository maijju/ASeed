// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/HUD/ASeedUI_LevelProgress.h"
#include "ASeedGameMode.generated.h"

/**
 * 
 */
USTRUCT()
struct FRewards
{
	GENERATED_BODY()

public:
	float ExpReward;
	float CreditReward;
};

UCLASS()
class ASEED_API AASeedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AASeedGameMode();

protected:
	virtual void BeginPlay() override;

protected:
	/*--------------PLAYER STATE--------------*/
	TObjectPtr<class AASeedPlayer> Player;
	TObjectPtr<class AASeedPlayerController> PlayerController;
	TArray<struct FLevelInfo> Levels;
	TArray<struct FLevelUpRewardInfo> LevelUpRewards;
	int32 CurrentLevel = 0;
	float CurrentExp = 0;
	float CurrentCredit = 0;

	/*--------------DATA--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName WaveKey = FName("Normal"); // Difficulty
	TSoftObjectPtr<UDataTable> WaveDataRef;
	UPROPERTY();
	UDataTable* WaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName LevelKey = FName("Default");
	TSoftObjectPtr<UDataTable> LevelDataRef;
	UPROPERTY();
	UDataTable* LevelData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName LevelUpRewardKey = FName("Default");
	TSoftObjectPtr<UDataTable> LevelUpRewardDataRef;
	UPROPERTY();
	UDataTable* LevelUpRewardData;

	/*--------------SPAWN--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 NumOfSpawnOffsets;
	// Enemies will be spawned nearby player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FVector> SpawnOffsets;

	/*--------------ENEMY WAVE--------------*/
	TArray<struct FWaveInfo> Waves;
	int32 CurrentWave = 0;
	int32 LastWave;
	FTimerHandle WaveTimerHandle;

	/*--------------UI--------------*/
	TObjectPtr<class UASeedUI_LevelProgress> LevelHUD;
	TObjectPtr<class UASeedUI_LevelUp> LevelUpUI;

public:
	void OnMainWidgetLoaded(UUserWidget* MainWidget);
	void ExecuteWave();
	void EarnEliminationRewards(const FRewards& Rewards);

protected:
	void LevelUp();
	UFUNCTION()
	void OnCardSelected(struct FCard Card);
	
};
