// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_Player_NormalAttack.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Effect/ASeedGE_Damage.h"

UASeedGA_Player_NormalAttack::UASeedGA_Player_NormalAttack()
{
	// NonInstanced : 인스턴스가 따로 생성되지 않고 클래스의 CDO만을 사용해서 동작시킨다.
	// InstancedPerActor : 각 액터마다 Ability 인턴스 1개를 생성한다. 각각의 액터가 자신의
	// 전용 어빌리티를 가지고 동작시키는 개념이다.
	// InstancedPerExecution : Ability가 발동될 때마다 새로운 인스턴스가 생성된다.
	// 공격이라면 공격 할 때마다 이 객체가 새로 생성되는 것이다.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;

	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.NormalAttack"));

	// GameplayEvent : GameplayEvent가 발생했을 때 동작한다.
	// 위에서 지정한 태그와 같은 태그가 담긴 이벤트가 ASC에 전달되면 Ability를 실행한다.
	// OwnedTagAdded : ASC의 Owned Gameplay Tags에 TriggerTag가 추가될 때 실행한다.
	// OwnedTagPresent : ASC에 이미 TriggerTag가 존재한다면 Ability를 항상 발동된
	// 상태로 간주한다.
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_Player_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AbilityActive)
	{
		return;
	}

	else if (!TriggerEventData || !TriggerEventData->Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Failed"));
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 전달받은 히트정보를 얻어온다.
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitData Failed"));
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
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
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UASeedAttributeSet* TargetAttr = TargetASC->GetSet<UASeedAttributeSet>();

	if (!TargetAttr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetAttr Failed"));
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player Attack Detected"));
	float	Attack = SourceAttr->GetAttack();
	float	Defense = TargetAttr->GetDefense();

	float	Dmg = Attack - Defense;

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
	DamageSpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Damage")),
		-Dmg);

	// 체력을 깎을 대상을 지정한다.
	/*FGameplayAbilityTargetDataHandle	TargetData;

	FGameplayAbilityTargetData_ActorArray* TargetArray =
		new FGameplayAbilityTargetData_ActorArray;

	TargetArray->TargetActorArray.Add(TargetActor);

	TargetData.Add(TargetArray);*/

	/*ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo,
		DamageSpecHandle, TargetData);*/

		// 위의 방법으로도 가능하고 아래의 방법으로도 가능하다.
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	/*FGameplayCueParameters	Param;
	Param.Instigator = GetAvatarActorFromActorInfo();
	Param.EffectCauser = GetOwningActorFromActorInfo();
	Param.Location = HitData->HitResult.ImpactPoint;

	SourceASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Player.Magician.Attack")), Param);*/


	// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

