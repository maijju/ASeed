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
	float CoreReward;
};

USTRUCT()
struct FModuleSummary
{
	GENERATED_BODY()

public:
	FName ModuleKey;
	FName ModuleName;
	FName ModuleType;
	FString ModuleDesc;
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
	TArray<struct FModuleInfo> Modules;
	TArray<struct FLevelUpRewardInfo> LevelUpRewards;
	int32 CurrentLevel = 0;
	int32 NumOfBuyModules = 0;
	float CurrentExp = 0;
	float CurrentCore = 0;
	FModuleSummary CurrentBulletModule;
	FModuleSummary CurrentSkillAModule;
	FModuleSummary CurrentSkillBModule;
	FModuleSummary SelectedModule;

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
	TObjectPtr<class UASeedUI_PlayerHUD> PlayerHUD;
	TObjectPtr<class UASeedUI_ModuleDetected> ModuleDetectedUI;
	TObjectPtr<class UASeedUI_ApplyBullet> ApplyBulletUI;
	TObjectPtr<class UASeedUI_ApplySkill> ApplySkillUI;
	TObjectPtr<class UASeedUI_Status> StatusUI;
	TObjectPtr<class UASeedUI_GameOver> GameOverUI;

public:
	void OnMainWidgetLoaded(UUserWidget* MainWidget);
	void ExecuteWave();
	void EarnEliminationRewards(const FRewards& Rewards);
	void TryInstallModule();
	UFUNCTION()
	void OnConfirmModule();
	UFUNCTION()
	void OnApplyBullet();
	UFUNCTION()
	void OnApplySkillA();
	UFUNCTION()
	void OnApplySkillB();
	UFUNCTION()
	void OnDiscardChange();

	void ShowStatus(const class UASeedAttributeSet* Attr);
	void CloseStatus();

	void GameOver();

protected:
	void LevelUp();
	UFUNCTION()
	void OnCardSelected(struct FCard Card);
	
};
