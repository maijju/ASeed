// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedTargetPawn.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "ASeedEnemyAnimInst.h"

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
	/*--------------DATA--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Key;
	TSoftObjectPtr<UDataTable> DataRef;
	UPROPERTY();
	UDataTable* Data;

	/*--------------LOCALS--------------*/
	TObjectPtr<UASeedEnemyAnimInst> AnimInst;
	float ExpReward;
	float CreditReward;

	/*--------------GAS--------------*/
	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<UASeedEnemyAttributeSet> AttributeSet;
	FGameplayTag AttackAbilityTag;
	TArray<FGameplayTag> AttackEffectTags;
	FGameplayTag AttackCueTag;

	/*--------------MOVEMENT--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* MovementComp;

	/*--------------AI--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	class UBehaviorTree* BehaviorTree;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return ASC;
	}

	void InitializeEnemy(FName EnemyKey);

	void ChangeAnimationState(EEnemyAnimState State)
	{
		if (!AnimInst)
			return;
		 AnimInst->SetAnimState(State);
	}
	void Attack();
	void OnDamage();
	UFUNCTION()
	void OnGameplayStun(const FGameplayTag Tag, int32 Count);
	void Die();
};
