// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AttributeSet.h"
#include "ASeedAttributeSet.generated.h"

#define	ATTRIBUTE_ACCESSORS(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ASEED_API UASeedAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString		mName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	AmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData	Gold;

public:
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Attack)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Ammo)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, AmmoMax)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Defense)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, HP)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, HPMax)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, AttackRange)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, AttackSpeed)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, MoveSpeed)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Level)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Exp)
	ATTRIBUTE_ACCESSORS(UASeedAttributeSet, Gold)

public:
	void SetName(const FString& Name)
	{
		mName = Name;
	}

	const FString& GetName()	const
	{
		return mName;
	}

public:
	// 인자로 들어오는 Data 의 속성을 체크해서 어떤 속성의 값이 변경되었는지를
	// 여기에서 체크할 수 있다.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data);

	virtual void CallbackAmmo();
	virtual void CallbackHP(bool IsDead);
	virtual void CallbackMP();
	virtual void CallbackAttack();
	virtual void CallbackDefense();
};
