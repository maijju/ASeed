// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Data/ASeedPlayerData.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"

#include "Components/ActorComponent.h"
#include "ASeedPlayerSkillComponent.generated.h"

UCLASS()
class ASEED_API UASeedPlayerSkillComponent: public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UASeedPlayerSkillComponent();

protected:
	TSoftObjectPtr<UDataTable> SkillDataRef;
	UPROPERTY();
	UDataTable* SkillData;

	FPlayerSkillData* SkillAData;
	FPlayerSkillData* SkillBData;

	TObjectPtr<class AASeedPlayer> Owner;
	TMap<FName, FPlayerSkillData> SkillMap;

private:
	virtual void BeginPlay() override;

public:
	void UpdateSkillADataByKey(FName SkillKey)
	{
		SkillAData = SkillData->FindRow<FPlayerSkillData>(SkillKey, TEXT(""));
	}
	void UpdateSkillBDataByKey(FName SkillKey)
	{
		SkillBData = SkillData->FindRow<FPlayerSkillData>(SkillKey, TEXT(""));
	}
	UDataTable* GetWholeSkillData()
	{
		return SkillData;
	}
	FPlayerSkillData* GetSkillAData()
	{
		return SkillAData;
	}
	FPlayerSkillData* GetSkillBData()
	{
		return SkillBData;
	}

	void TryActivateSkill(bool IsSkillA);
};
