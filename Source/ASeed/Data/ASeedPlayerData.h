// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"

#include "ASeedPlayerData.generated.h"

/*---------------------------------------*/
/*--------------PLAYER STAT--------------*/ 
/*---------------------------------------*/
USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString		Name;

	UPROPERTY(EditAnywhere)
	int32		Attack;

	UPROPERTY(EditAnywhere)
	int32		Ammo;

	UPROPERTY(EditAnywhere)
	int32		AmmoMax;

	UPROPERTY(EditAnywhere)
	int32		Defense;

	UPROPERTY(EditAnywhere)
	float		HP;

	UPROPERTY(EditAnywhere)
	float		HPMax;

	UPROPERTY(EditAnywhere)
	float		AttackRange;

	UPROPERTY(EditAnywhere)
	float		AttackSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float		MoveSpeed = 250.f;

	UPROPERTY(EditAnywhere)
	int32		Level = 1;

	UPROPERTY(EditAnywhere)
	int32		Exp = 0;

	UPROPERTY(EditAnywhere)
	int32		Gold = 0;
};

/*---------------------------------------*/
/*--------------PLAYER SKILL-------------*/
/*---------------------------------------*/
UCLASS(BlueprintType)
class ASEED_API UASeedPlayerSkillCost : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float Cooldown;
	UPROPERTY()
	float AmmoCost;
};

USTRUCT(BlueprintType)
struct ASEED_API FPlayerSkillData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	float AmmoCost;
};

/*---------------------------------------*/
/*--------------BULLET DATA--------------*/
/*---------------------------------------*/
UCLASS()
class ASEED_API UASeedPlayerBulletData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector Location;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayEffectTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayMuzzleFlashCueTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayBulletHitCueTag;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere)
	int32 PierceCount;
};

USTRUCT(BlueprintType)
struct ASEED_API FPlayerBulletData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayEffectTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayMuzzleFlashCueTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayBulletHitCueTag;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere)
	int32 PierceCount;
};