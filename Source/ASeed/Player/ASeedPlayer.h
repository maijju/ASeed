// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASeedPlayerAnimInst.h"
#include "../Weapon/ASeedTestBullet.h"
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

	/*--------------EFFECT--------------*/
	TObjectPtr<UParticleSystemComponent> ParticleComp;
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* FireEffect;

	/*--------------Data--------------*/
	UPROPERTY(EditAnywhere, Category = "Data")
	FPlayerData PlayerData;

	/*--------------LOCAL VARIABLES--------------*/
	TObjectPtr<UASeedPlayerAnimInst> AnimInst;
	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<UASeedPlayerAttributeSet> AttributeSet;
	TObjectPtr<AASeedTargetPawn> CachedTarget;
	FName CurrentWeaponSocketName;

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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return ASC;
	}

	void Fire(FName SocketName);
	void Reloaded();

	void OnAmmoModified();
	void OnDamage();
	void Die();
	void OnGameplayStun()
	{

	}

public:
	FName GetSocketName()
	{
		return CurrentWeaponSocketName;
	}

	void SetSocketName(FName SocketName)
	{
		CurrentWeaponSocketName = SocketName;
	}

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
