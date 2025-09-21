// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "ASeedGE_ModifyAmmo.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGE_ModifyAmmo : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UASeedGE_ModifyAmmo();
};
