// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedAbility.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Effect/ASeedGE_Damage.h"
#include "../Effect/ASeedGE_ConsumeAmmo.h"
#include "../Effect/ASeedGE_Cooldown.h"

UASeedAbility::UASeedAbility()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	// 이 어빌리티가 발동되지 못하도록 막는 태그 집합이다.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Custom.State.Stun")));
}

// FGameplayAbilitySpecHandle : AbilitySpec을 식별하는 고유한 핸들(값)
// ASC(AbilitySystemComponent)가 소유한 AbilitySpec을 가리킨다.
// FGameplayAbilityActorInfo : Ability가 실행되는 주체에 대한 정보.
// 즉, ASC를 가지고 있는 OwnerActor 정보를 표현한 구조체이다.
// FGameplayAbilityActivationInfo : 이번 Ability 실행이 어떤 방식으로 트리거 되었는지에
// 대한 메타 정보.
// ActivationMode : Ability가 어떻게 발동되었는지.(키를 눌렀는지 등등)
// PredictionKey : 네트워크 예측용 키(클라이언트와 서버의 동기화에 사용)
// bCanBeEndedByOtherInstance : 다른 인스턴스에 의해 종료될 수 있는지 여부
// FGameplayEventData : Ability가 GameplayEvent에 의해 발생되었을 때, 이벤트와 함께
// 전달된 데이터. 히트 정보 등등.
void UASeedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 일단 발동상태로 만들어두고 발동이 불가능한 상황이라면 false로 만들어준다.
	bActive = true;

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		bActive = false;
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	AActor* SourceActor = GetAvatarActorFromActorInfo();

	// SourceActor가 가지고 있는 AbilitySystemComponent를 얻어온다.
	//UAbilitySystemComponent* SourceASC = Cast<IAbilitySystemInterface>(SourceActor)->GetAbilitySystemComponent();

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// AttributeSet을 얻어온다.
	const UASeedAttributeSet* SourceAttr = SourceASC->GetSet<UASeedAttributeSet>();

	if (!SourceAttr)
	{
		bActive = false;
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 비용소모가 불가능할 경우 어빌리티 사용 중지.
	for (auto& Cost : CostArray)
	{
		switch (Cost.Type)
		{
		case ECostType::Ammo:
			if (SourceAttr->GetAmmo() < Cost.Cost)
			{
				bActive = false;
				EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
				return;
			}
			break;
		case ECostType::HP:
			if (SourceAttr->GetHP() <= Cost.Cost)
			{
				bActive = false;
				EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
				return;
			}
			break;
		}
	}

	// 쿨다운을 체크한다.
	if (Cooldown > 0.f)
	{
		float CooldownAmount = Cooldown;

		// .스킬 쿨감 20% 감소일 경우
		//CooldownAmount *= (1.f - 0.2f);

		FGameplayEffectSpecHandle	CooldownSpec = MakeOutgoingGameplayEffectSpec(UASeedGE_Cooldown::StaticClass(), 1.f);
		CooldownSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Cooldown")), CooldownAmount);
		// 동적으로 추가되는 태그를 저장하는 컨테이너이다.
		CooldownSpec.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));
		SourceASC->ApplyGameplayEffectSpecToSelf(*CooldownSpec.Data);
	}

	// 소모비용을 모두 통과했다면 비용을 소모시킨다.
	int32 AmmoCost = 0;
	float HPCost = 0.f;
	for (auto& Cost : CostArray)
	{
		switch (Cost.Type)
		{
		case ECostType::Ammo:
		{
			AmmoCost += Cost.Cost;
		}
		break;
		case ECostType::HP:
		{
			HPCost += Cost.Cost;
		}
		break;
		}
	}

	if (AmmoCost > 0)
	{
		FGameplayEffectSpecHandle AmmoCostSpec = MakeOutgoingGameplayEffectSpec(UASeedGE_ConsumeAmmo::StaticClass(), GetAbilityLevel());
		AmmoCostSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.ConsumeAmmo")), -AmmoCost);
		SourceASC->ApplyGameplayEffectSpecToSelf(*AmmoCostSpec.Data);
	}

	if (HPCost > 0.f)
	{
		FGameplayEffectSpecHandle HPCostSpec = MakeOutgoingGameplayEffectSpec(UASeedGE_Damage::StaticClass(), GetAbilityLevel());
		HPCostSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Damage")), -HPCost);
		SourceASC->ApplyGameplayEffectSpecToSelf(*HPCostSpec.Data);
	}
}
