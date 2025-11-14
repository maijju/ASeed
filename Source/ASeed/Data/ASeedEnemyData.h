// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "Engine/SkeletalMesh.h"
#include "../Enemy/ASeedEnemyAnimInst.h"
#include "ASeedEnemyData.generated.h"

/*---------------------------------------*/
/*--------------ENEMY STAT---------------*/
/*---------------------------------------*/
USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UASeedEnemyAnimInst> ABP;

	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimSequence>> SeqMap;

	UPROPERTY(EditAnywhere)
	bool bHasProjectile;

	UPROPERTY(EditAnywhere)
	bool bIsBossEnemy;

	UPROPERTY(EditAnywhere)
	FName BulletKey;

	UPROPERTY(EditAnywhere)
	FGameplayTag AttackAbilityTag;

	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> AttackEffectTags;

	UPROPERTY(EditAnywhere)
	FGameplayTag AttackCueTag;

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
/*--------------BULLET DATA--------------*/
/*---------------------------------------*/
UCLASS()
class ASEED_API UASeedEnemyBulletData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector Location;

	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> GameplayEffectTags;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayMuzzleFlashCueTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayBulletHitCueTag;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere)
	float EffectDuration;

	UPROPERTY(EditAnywhere)
	float BulletSpeed;
};

USTRUCT(BlueprintType)
struct ASEED_API FEnemyBulletData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName BulletName;

	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> GameplayEffectTags;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayMuzzleFlashCueTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayBulletHitCueTag;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere)
	float EffectDuration;

	UPROPERTY(EditAnywhere)
	float BulletSpeed = 1000.f;
};