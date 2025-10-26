// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Enemy/ASeedEnemy.h"
#include "ASeedPlayerData.h"
#include "ASeedGameData.generated.h"

/*---------------------------------------*/
/*---------------WAVE DATA---------------*/
/*---------------------------------------*/
USTRUCT(BlueprintType)
struct FWaveInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName EnemyKey;
	UPROPERTY(EditAnywhere)
	int32 EnemyCount;
	UPROPERTY(EditAnywhere)
	float TimeToCallNextWave;
	UPROPERTY(EditAnywhere)
	bool bIsBossWave;
};

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FWaveInfo> Waves;
};

/*---------------------------------------*/
/*--------------LEVEL DATA---------------*/
/*---------------------------------------*/
USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MaxExp;
};

USTRUCT(BlueprintType)
struct FModuleInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MaxCore;
};

USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FLevelInfo> Levels;

	UPROPERTY(EditAnywhere)
	TArray<FModuleInfo> Modules;
};

/*---------------------------------------*/
/*---------LEVEL UP REWARD DATA----------*/
/*---------------------------------------*/
USTRUCT(BlueprintType)
struct FLevelUpRewardInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FText StatName;
	UPROPERTY(EditAnywhere)
	EPlayerStats StatEnum;
	UPROPERTY(EditAnywhere)
	int32 MinValue;
	UPROPERTY(EditAnywhere)
	int32 MaxValue;
	UPROPERTY(EditAnywhere)
	FString Description;
};

USTRUCT(BlueprintType)
struct FLevelUpRewardData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FLevelUpRewardInfo> LevelUpRewards;
};