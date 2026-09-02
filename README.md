# A Seed

<img width="937" height="472" alt="image" src="https://github.com/user-attachments/assets/a44a6ba1-4444-4d98-94ef-540fed72c7f4" />


> 데이터 기반 설계와 GAS(Gameplay Ability System)를 결합하여 모듈화 및 확장성에 집중한 프로젝트입니다.

- **장르**: 덱빌딩 로그라이크 탑다운 슈팅
- **개발 환경**: Unreal Engine 5.5.4, C++, VisualStudio
- **개발 인원**: 개인 프로젝트 (1인 개발)
- **주요 링크**:
  - 📝 [티스토리 데브로그](https://maiju.tistory.com/category/Projects/A%20Seed)
  - 🎬 [YouTube 데모 플레이 영상](https://youtu.be/_-OkF2EwRps)
  - 📄 [Notion 프로젝트 페이지](https://www.notion.so/UE5-Personal-Project-A-Seed-266fad82004d809f811bf5e85c265aaf)

---

## 게임 소개
- 유저의 선택과 플레이 스타일이 실시간 스킬/스탯으로 반영되는 탑다운 디펜스 게임입니다.
- 게임 진행 중 무기 강화, 스탯 증대, 신규 스킬을 습득하는 **덱빌딩 요소**를 융합했습니다.
- 다양한 컨텐츠가 재미와 직결될 것으로 생각해서, 기획 요소가 추가되더라도 코드 수정 없이 에디터 레벨에서 빠르게 대응할 수 있도록 설계했습니다.

---

## 핵심 구현 컨텐츠 및 아키텍처

### 1. 게임 컨텐츠와 GAS 핵심 요소 1:1 매칭
빠른 모듈화와 높은 유지보수성을 달성하기 위해, 게임 내 성장 및 스킬 시스템을 GAS의 핵심 요소와 **1:1로 매칭**하여 설계했습니다.

| 분류 | 레벨 업 | 모듈 장착 (스킬 모듈) | 모듈 장착 (탄환 모듈) |
| :--- | :--- | :--- | :--- |
| **역할** | 기본 스탯 수치 증가 | **액티브 스킬** 부여 (예: 구르기, 멀티샷 등) | **패시브 스킬** 부여 (예: 충격탄, 작열탄 등) |
| **획득 경로** | 레벨업 시 3개 중 택 1 (수치 랜덤) | 코어 획득 후 수동 설치 (가챠 시스템) | 코어 획득 후 수동 설치 (가챠 시스템) |
| **매칭 기술** | **AttributeSet** | **Gameplay Ability** | **Gameplay Effect** |

- **스킬 모듈**: `GameplayAbility`로 구현하여 **탄환 발사**, **액티브 스킬** 등을 구현했습니다.
- **탄환 모듈**: `GameplayEffect` 및 `GameplayTag` 배열로 관리되어, 발사체 충돌 시 적에게 **데미지**, **버프/디버프**를 유연하게 적용합니다.

다음은 주요 코드 요약 (탄환 발사 Gameplay Ability, 디버프 Gameplay Effect) 입니다.

> ASeedGA_PlayerFire.cpp
```cpp
void UASeedGA_PlayerFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	/*--------------INIT--------------*/
	AASeedPlayer* Player = Cast<AASeedPlayer>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();

	if (!Player || !PlayerASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UASeedPlayerBulletData* BulletData = Cast<UASeedPlayerBulletData>(TriggerEventData->OptionalObject);
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletData is nullptr (in ability)"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	FVector MuzzleLocation = BulletData->Location;

	/*--------------SPAWN BULLET--------------*/
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AASeedPlayerBullet* Bullet = GetWorld()->SpawnActor<AASeedPlayerBullet>(MuzzleLocation, Player->GetActorRotation(), Param);
	Bullet->SetDamage(PlayerASC->GetSet<UASeedAttributeSet>()->GetAttack());
	Bullet->SetBulletData(BulletData);
	Bullet->SetOwnerController(Player->GetController());
	Bullet->Body->SetGenerateOverlapEvents(true);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
```

> ASeedGE_Stun.cpp

```cpp
UASeedGE_Stun::UASeedGE_Stun()
{
    DurationPolicy = EGameplayEffectDurationType::HasDuration;
    FSetByCallerFloat DurationCaller;
    DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Custom.Effect.StunDuration"));
    DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);
}
```
(Gameplay Effect 적용은 총알 Hit Ability에 구현되어있습니다.)

- **성과**: 플레이어나 적 등 능력을 사용하는 주체가 쿨타임, 코스트 소모 등의 로직을 전담하지 않는 책임의 분배가 원활히 이루어졌습니다.

---

### 2. DataTable 기반의 Data-Driven 구조 (에디터 친화적 개발 환경 조성)

`UDataTable`과 `SoftObjectPath/SoftObjectPtr`를 적극 활용하여 **C++ 코드 수정 없이 에디터 데이터 등록만으로 적(Enemy), 총알(Bullet), 스킬 배리언트를 즉시 인게임에 생성 및 적용**하는 파이프라인을 구축했습니다.

다음은 주요 코드 요약 (Enemy가 데이터 테이블 연동하여 종류를 가지게 되는 과정) 입니다.
> ASeedEnemyData.h

```cpp
/*--------------------------------------------------
   Enemy Data Structure (DataTable)
--------------------------------------------------*/
USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditAnywhere) TSubclassOf<UASeedEnemyAnimInst> ABP;
	UPROPERTY(EditAnywhere) TMap<FName, TObjectPtr<UAnimSequence>> SeqMap;

	UPROPERTY(EditAnywhere) bool bHasProjectile;
	UPROPERTY(EditAnywhere) bool bIsBossEnemy;
	UPROPERTY(EditAnywhere) FName BulletKey;

	// GAS Tags Dynamic Binding
	UPROPERTY(EditAnywhere) FGameplayTag AttackAbilityTag;
	UPROPERTY(EditAnywhere) TArray<FGameplayTag> AttackEffectTags;
	UPROPERTY(EditAnywhere) FGameplayTag AttackCueTag;

	// Stats
	UPROPERTY(EditAnywhere) float HP;
	UPROPERTY(EditAnywhere) float HPMax;
	UPROPERTY(EditAnywhere) int32 Attack;
	UPROPERTY(EditAnywhere) float AttackRange;
	UPROPERTY(EditAnywhere) float MoveSpeed = 250.f;
};
```

> ASeedEnemy.cpp
```cpp
/*--------------------------------------------------
   Dynamic Enemy Initialization from DataTable
--------------------------------------------------*/
void AASeedEnemy::InitializeEnemy(FName EnemyKey)
{
	Data = DataRef.LoadSynchronous();
	FEnemyData* Row = Data->FindRow<FEnemyData>(EnemyKey, TEXT(""));
	if (!Row) return;

	// 1. 에셋 및 애니메이션 동적 바인딩
	MeshComp->SetSkeletalMesh(Row->Mesh);
	MeshComp->SetAnimInstanceClass(Row->ABP);
	AnimInst = Cast<UASeedEnemyAnimInst>(MeshComp->GetAnimInstance());
	AnimInst->InitSeqMap(Row->SeqMap);

	// 2. AttributeSet 스탯 데이터 동적 주입
	AttributeSet->SetHP(Row->HP);
	AttributeSet->SetHPMax(Row->HPMax);
	AttributeSet->SetAttack(Row->Attack);
	AttributeSet->SetAttackRange(Row->AttackRange);
	AttributeSet->SetMoveSpeed(Row->MoveSpeed);

	// 3. AI 및 Behavior Tree 변수 동적 설정
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (Player && AIController && BehaviorTree)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Player);
		AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), Row->AttackRange);
		MovementComp->MaxSpeed = Row->MoveSpeed;
	}
}
```

- **성과**: 에셋 경로와 스탯, 스킬 태그, AI 매개변수를 데이터 테이블로 완전히 분리했습니다. 신규 적 클래스/보스/탄환 추가 시 C++ 재컴파일 없이 에디터에서 테이블 행만 추가하면 인게임에 즉각 반영됩니다.

### 3. 기타 핵심 구현 기술
- **HLSL Outliner Material**: 커스텀 HLSL 셰이더를 작성하여 렌더링 가시성 및 타겟팅 라인 구현.
- **BehaviorTree & Blackboard AI**: 거리 계산, 타겟팅, 스턴(Stun) 상태 제어를 포함한 적 AI 제어.
- **Niagara VFX & GameplayCue**: 플레이어 이동 잔상(Trail) 및 타격/발사 특수 효과 시각화.
- **AssetManager & UMG**: 비동기 로딩 관리 및 체력바/상태창 UI 연동.

---

## 트러블슈팅 (Troubleshooting)

### 1. 관통 탄환 구현 시 충돌 판정 및 다중 타격 오작동
- **문제**: 기존 탄환은 물리 충돌(`Block`) 시 발생하는 `OnProjectileStop` 이벤트에 의존하여, 적 개체(`Overlap`)를 통과하며 관통 다중 타격을 수행할 수 없는 구조였습니다.
- **해결**:
  1. 충돌 프로필을 `PlayerAttack`으로 세분화하고 콜백을 `OnComponentBeginOverlap`으로 재바인딩했습니다.
  2. `PierceCount` 카운터를 도입하여, 지정된 관통 횟수를 초과하기 전까지 탄환이 파괴되지 않고 연속 `Overlap` 이벤트를 발생시키도록 변경했습니다.

### 2. GAS Event Data 단일 태그 제약으로 인한 복수 효과(Effect) 적용 제한
- **문제**: 탄환 발사 Ability 수행 시 단일 `FGameplayTag`만 페이로드에 전달되어, "화염 데미지 + 둔화"처럼 한 번의 공격에 여러 `GameplayEffect`를 동시에 입히는 덱빌딩 시너지를 구현할 수 없었습니다.
- **해결**:
  1. 발사체 컴포넌트의 데이터 페이로드를 `TArray<FGameplayTag>` 구조로 리팩토링했습니다.
  2. `HandleGameplayEvent` 호출 시 태그 배열 전체를 `FGameplayEventData`로 패킹하여 전송함으로써, 단일 탄환으로 유연한 다중 상태이상을 적에게 적용할 수 있도록 확장했습니다.

---

## 개발 회고 및 성찰

- **GAS 설계 패턴을 몸소 체험했습니다.**:
  `Ability`, `Effect`, `AttributeSet`을 독립적으로 설계하면서 모듈화된 아키텍처의 중요성을 체감했습니다. 코드 간 결합도를 낮추어 기능 추가나 밸런스 수정 시 발생하는 사이드 이펙트를 최소화할 수 있었습니다.
- **언리얼 생태계에 대한 이해도를 높였습니다.**:
  초기에는 데이터 테이블 로딩 시점과 GAS 초기화(`InitAbilityActorInfo`) 순서가 꼬여 Attribute 값이 반영되지 않거나, 비동기 데이터 로딩 과정에서 널 참조가 발생하는 이슈가 있었습니다. Lifecycle 단계(`BeginPlay`, `OnConstruction`)에 맞춰 초기화 순서를 정교하게 동기화함으로써 안정적인 생성 흐름을 구축했습니다.
