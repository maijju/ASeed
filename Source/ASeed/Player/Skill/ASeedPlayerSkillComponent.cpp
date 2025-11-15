// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerSkillComponent.h"
#include "../ASeedPlayer.h"
#include "../../Ability/Skill/ASeedGA_PlayerRolling.h"
#include "../../Ability/Skill/ASeedGA_PlayerSpread.h"
#include "../../Ability/Skill/ASeedGA_PlayerBoost.h"
#include "../../Ability/Skill/ASeedGA_PlayerRampage.h"
#include "../../Ability/Skill/ASeedGA_PlayerSlowmotion.h"
#include "../../ASeedGameMode.h"

UASeedPlayerSkillComponent::UASeedPlayerSkillComponent()
{
	SkillDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/DT_PlayerSkill.DT_PlayerSkill'"));
}

void UASeedPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AASeedPlayer>(GetOwner());

	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerRolling::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerSpread::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerBoost::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerSlowmotion::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerRampage::StaticClass(), 1, INDEX_NONE, this));

	SkillData = SkillDataRef.LoadSynchronous();
	if (!SkillData)
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SkillData"));
}

void UASeedPlayerSkillComponent::TryActivateSkill(bool IsSkillA)
{
	if (!Owner)
		return;

	FPlayerSkillData* Row;

	if (IsSkillA)
	{
		Row = SkillAData;
	}
	else
	{
		Row = SkillBData;
	}


	if (Owner->GetPlayerAnimInstance()->IsAnyMontagePlaying())
	{
		if (!Row->CanCancelAttack)
			return;
		else
		{
			if (!Owner->GetPlayerAnimInstance()->IsPlayingAttack())
				return;
		}
	}

	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();

	if (ASC)
	{
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SkillData"));
			return;
		}

		TObjectPtr<UASeedPlayerSkillCost> SkillCost = NewObject<UASeedPlayerSkillCost>();
		SkillCost->Cooldown = Row->Cooldown;
		SkillCost->AmmoCost = Row->AmmoCost;

		FGameplayEventData EventData;
		EventData.Instigator = Owner;
		EventData.OptionalObject = SkillCost;

		FGameplayTag SkillTag = Row->AbilityTag;
		ASC->HandleGameplayEvent(SkillTag, &EventData);

		AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
		GM->SetSkillCool(IsSkillA, Row->Cooldown);
	}
}