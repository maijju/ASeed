// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Abilities/GameplayAbility.h"
#include "ASeedAbility.generated.h"

UENUM(BlueprintType)
enum class ECostType : uint8
{
	Ammo,
	HP
};

USTRUCT(BlueprintType)
struct FAbilityCost
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	ECostType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float		Cost;
};

UCLASS()
class ASEED_API UASeedAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UASeedAbility();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<FAbilityCost>	CostArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Cooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FName CooldownTag;

	bool bActive = false;

public:
	void AddAbilityCost(ECostType Type, float Cost)
	{
		FAbilityCost	Result;
		Result.Type = Type;
		Result.Cost = Cost;
		CostArray.Add(Result);
	}

	void SetCooldown(float Time)
	{
		Cooldown = Time;
	}

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
};
