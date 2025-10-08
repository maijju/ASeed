// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	TMap<FName, TObjectPtr<UAnimSequence>> SeqMap;

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