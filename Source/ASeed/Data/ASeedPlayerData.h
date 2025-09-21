// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ASeedPlayerData.generated.h"

// Format For Player Stats
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
	int32		HP;

	UPROPERTY(EditAnywhere)
	int32		HPMax;

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