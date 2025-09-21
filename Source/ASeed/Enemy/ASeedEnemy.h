// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedTargetPawn.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"

#include "../AttributeSet/ASeedEnemyAttributeSet.h"

#include "ASeedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API AASeedEnemy : public AASeedTargetPawn, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AASeedEnemy();

protected:
	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<UASeedEnemyAttributeSet> AttributeSet;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return ASC;
	}

	void OnDamage();
};
