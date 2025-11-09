// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASeedPlayerAnimInst.h"

#include "Projectile/ASeedPlayerBullet.h"
#include "Projectile/ASeedPlayerProjectileComponent.h"

#include "Skill/ASeedPlayerSkillComponent.h"

#include "VFX/ASeedPlayerVFXComponent.h"

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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHit, float, CurrentHp, float, MaxHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseAmmo, float, CurrentAmmo, float, MaxAmmo);

UCLASS()
class ASEED_API AASeedPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerHit OnPlayerHit;

	UPROPERTY(BlueprintAssignable)
	FOnUseAmmo OnUseAmmo;

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
	UInputAction* SkillAAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillBAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InstallModuleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShowStatusAction;

	/*--------------STAT--------------*/
	UPROPERTY(EditAnywhere, Category = "Stat")
	FPlayerData PlayerData;

	/*--------------BULLET--------------*/
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UASeedPlayerProjectileComponent* ProjComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BulletKey = FName("Default");

	/*--------------SKILL--------------*/
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	UASeedPlayerSkillComponent* SkillComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillAKey = FName("Empty");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillBKey = FName("Empty");

	/*--------------VFX--------------*/
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UASeedPlayerVFXComponent* VFXComp;

	/*--------------LOCAL VARIABLES--------------*/
	TObjectPtr<class AASeedGameMode> GM;
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
	void InstallModule();
	void ShowStatus();
	void CloseStatus();

public:
	void Fire(FName SocketName);
	void Reloaded();

	void SkillA();
	void SkillB();

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
	void OnDamage(bool IsDead);
	void OnHPMaxChanged();
	void OnAmmoChanged();
	void OnAmmoMaxChanged();
	void OnGameplayStun()
	{

	}
	UASeedPlayerProjectileComponent* GetProjectileComponent()
	{
		return ProjComp;
	}
	UASeedPlayerSkillComponent* GetSkillComponent()
	{
		return SkillComp;
	}
	UASeedPlayerVFXComponent* GetVFXComponent()
	{
		return VFXComp;
	}
	void UpdateBulletKey(FName Key)
	{
		BulletKey = Key;
		ProjComp->UpdateBulletDataByKey(Key);
	}
	void UpdateSkillAKey(FName Key)
	{
		SkillAKey = Key;
		SkillComp->UpdateSkillADataByKey(Key);
	}
	void UpdateSkillBKey(FName Key)
	{
		SkillBKey = Key;
		SkillComp->UpdateSkillBDataByKey(Key);
	}
	void SetRotationFreeze(bool Enable)
	{
		bRotationFreeze = Enable;
	}

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
