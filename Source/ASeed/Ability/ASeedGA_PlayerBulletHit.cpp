// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerBulletHit.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Effect/ASeedGE_Damage.h"

UASeedGA_PlayerBulletHit::UASeedGA_PlayerBulletHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.BulletHit"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_PlayerBulletHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		return;
	}

	else if (!TriggerEventData || !TriggerEventData->Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 전달받은 히트정보를 얻어온다.
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitData Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	AActor* SourceActor = GetAvatarActorFromActorInfo();

	// 이 어빌리티를 발생시킨 ASC를 얻어온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// AttributeSet을 얻어온다.
	const UASeedAttributeSet* SourceAttr = SourceASC->GetSet<UASeedAttributeSet>();

	if (!SourceAttr)
	{
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 공격처리.
	AActor* TargetActor = HitData->HitResult.GetActor();

	// Target의 ASC를 얻어온다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetASC Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UASeedAttributeSet* TargetAttr = TargetASC->GetSet<UASeedAttributeSet>();

	if (!TargetAttr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetAttr Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	float	Attack = SourceAttr->GetAttack();
	float	Defense = TargetAttr->GetDefense();
	float	Dmg = Attack - Defense;
	FGameplayTag EffectTag = TriggerEventData->TargetTags.GetByIndex(0);

	Dmg = FMath::Max(1.f, Dmg);

	// SetByCaller로 되어 있는 값을 지정하고 Effect를 호출하기 위해
	// EffectContextHandle을 만들어준다.
	FGameplayEffectContextHandle	ContextHandle =
		MakeEffectContext(Handle, ActorInfo);

	// 만약 Effect로 충돌정보를 넘겨줘야 한다면
	ContextHandle.AddHitResult(HitData->HitResult);

	// Damage Effect에 대한 Handle을 만들어준다.
	//TargetASC->MakeOutgoingSpec()
	FGameplayEffectSpecHandle	DamageSpecHandle =
		MakeOutgoingGameplayEffectSpec(UASeedGE_Damage::StaticClass(), 1.f);

	// 위에서 생성한 핸들을 지정한다.
	DamageSpecHandle.Data->SetContext(ContextHandle);

	// SetByCaller에 들어갈 데미지를 지정한다.
	// 체력을 깎아야 하기 때문에 -를 붙여준다.
	DamageSpecHandle.Data->SetSetByCallerMagnitude(EffectTag, -Dmg);

		// 위의 방법으로도 가능하고 아래의 방법으로도 가능하다.
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	/*--------------CUE--------------*/
	FGameplayCueParameters CueParam;
	FGameplayTag CueTag = TriggerEventData->TargetTags.GetByIndex(1);
	CueParam.Instigator = GetAvatarActorFromActorInfo();
	CueParam.EffectCauser = GetOwningActorFromActorInfo();
	CueParam.Location = HitData->HitResult.ImpactPoint;

	SourceASC->ExecuteGameplayCue(CueTag, CueParam);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
