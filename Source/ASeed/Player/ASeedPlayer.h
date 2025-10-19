// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASeedPlayerAnimInst.h"

#include "Projectile/ASeedPlayerBullet.h"
#include "Projectile/ASeedPlayerProjectileComponent.h"
#include "Skill/ASeedPlayerSkillComponent.h"

#include "../AttributeSet/ASeedPlayerAttributeSet.h"
#include "../Data/ASeedPlayerData.h"

#include "../Enemy/ASeedTargetPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "ASeedPlayer.generated.h"

UCLASS()
class ASEED_API AASeedPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	/*--------------CAMERA--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/*--------------INPUT--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill2Action;

	/*--------------STAT--------------*/
	UPROPERTY(EditAnywhere, Category = "Stat")
	FPlayerData PlayerData;

	/*--------------BULLET--------------*/
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	TObjectPtr<UASeedPlayerProjectileComponent> ProjComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BulletKey = FName("Default");

	/*--------------SKILL--------------*/
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	TObjectPtr<UASeedPlayerSkillComponent> SkillComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Skill1Key = FName("Rolling");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Skill2Key;

	/*--------------LOCAL VARIABLES--------------*/
	TObjectPtr<UASeedPlayerAnimInst> AnimInst;
	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<UASeedPlayerAttributeSet> AttributeSet;
	TObjectPtr<AASeedTargetPawn> CachedTarget;
	FVector2D CurrentKeyVector;
	bool bRotationFreeze = false;

public:
	AASeedPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void Attack(); // pair with Fire
	void Reload(); // pair with Reloaded

public:
	void Fire(FName SocketName);
	void Reloaded();

	void Skill1();
	void Skill2();

	void Die();

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return ASC;
	}

	UASeedPlayerAnimInst* GetPlayerAnimInstance()
	{
		return AnimInst;
	}

	FVector2D GetCurrentKeyVector()
	{
		return CurrentKeyVector;
	}

public:
	void OnDamage();
	void OnGameplayStun()
	{

	}

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
