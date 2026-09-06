# A Seed

<img width="937" height="472" alt="image" src="https://github.com/user-attachments/assets/a44a6ba1-4444-4d98-94ef-540fed72c7f4" />


> 데이터 기반 설계와 GAS(Gameplay Ability System)를 결합하여 모듈화 및 확장성에 집중한 프로젝트입니다.

- **장르**: 덱빌딩 로그라이크 탑다운 슈팅
- **개발 환경**: Unreal Engine 5.5.4, C++, VisualStudio
- **개발 인원**: 개인 프로젝트 (1인 개발)
- **개발 기간**: 2개월 (2025.09.13~2025.11.14)
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

## 핵심 구현 컨텐츠

### 1. 게임 컨텐츠와 GAS 핵심 요소 1:1 매칭
- **빠른 모듈화와 높은 유지보수성을 달성하기 위해, 게임 내 성장 및 스킬 시스템을 GAS의 핵심 요소와 1:1로 매칭하여 설계했습니다.**

| 분류 | 레벨 업 | 모듈 장착 (스킬 모듈) | 모듈 장착 (탄환 모듈) |
| :--- | :--- | :--- | :--- |
| **역할** | 기본 스탯 수치 증가 | **액티브 스킬** 부여 (예: 구르기, 멀티샷 등) | **패시브 스킬** 부여 (예: 충격탄, 작열탄 등) |
| **획득 경로** | 레벨업 시 3개 중 택 1 (수치 랜덤) | 코어 획득 후 수동 설치 (가챠 시스템) | 코어 획득 후 수동 설치 (가챠 시스템) |
| **매칭 기술** | **AttributeSet** | **Gameplay Ability** | **Gameplay Effect** |

**스킬 모듈**: `GameplayAbility`로 구현하여 **탄환 발사**, **액티브 스킬** 등을 구현했습니다.  
**탄환 모듈**: `GameplayEffect` 및 `GameplayTag` 배열로 관리되어, 발사체 충돌 시 적에게 **데미지**, **버프/디버프**를 유연하게 적용합니다.

> 스킬 모듈 예시 (구르기)

![스킬](/readme_gifs/skil.gif)

> 탄환 모듈 예시 (충격탄)

![탄환](/readme_gifs/bullet.gif)


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

<br>

### 2. DataTable 기반의 Data-Driven 구조

- **컨텐츠 확장성 극대화**: `UDataTable`과 `SoftObjectPath/SoftObjectPtr`를 적극 활용하여 C++ 코드 수정 없이 에디터 데이터 등록만으로 적(Enemy), 총알(Bullet), 스킬 배리언트를 즉시 인게임에 생성 및 적용하는 파이프라인을 구축했습니다.
- **`FGameplayTag`를 키로 활용하여 GAS와 연동되는 데이터 테이블**: `FGameplayTag`로 GAS의 `GameplayAbility`, `GameplayEffect`를 에디터에서 선택할 수 있도록 설정하여 플레이어가 얻을 수 있는 보상을 에디터 선에서 생성할 수 있는 환경을 만들었습니다.

<img width="1038" height="284" alt="image" src="https://github.com/user-attachments/assets/9eecfbc8-4312-466f-b453-bcbcbfe379ef" />


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

<br>

### 3. UMG를 활용한 전체 HUD및 UI 제작
- **`UUserWidget` 파생 클래스를 작성한 뒤 블루프린트로 디자인**: UMG의 `UserWidget`을 활용하여 버튼, 텍스트, 프로그래스 바 등의 기능을 구현했습니다.
- **`AGameMode`가 UI 데이터를 제공하는 GameManager역할을 하도록 설계**: UI가 여러 객체를 참조하게 되는 구조를 피하기 위해 커스텀 `GameMode`에 주요 자원(레벨, 모듈)등을 캐싱하고 UI가 이 데이터를 기반하여 표시하도록 설계했습니다.
- **UI전용 컨테이너 구조체를 구축하여 반복되는 데이터에 활용**: `FCard` 구조체를 만들고 레벨업 보상을 표현하도록 설계하여 for loop로 UI를 화면에 생성하는 구조를 구현했습니다. 카드 클릭 시 함수는 언리얼의 델리게이트를 활용하여 Broadcast 했습니다.

다음은 주요 코드 요약 (레벨업 보상 선택지 UI) 입니다.

> ASeedUI_LevelUp.cpp
```c++
#include "ASeedUI_LevelUp.h"

void UASeedUI_LevelUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cards.SetNum(3);

	for (int32 i=0; i<3; i++)
	{
		FString CardId = FString::Printf(TEXT("Card%d"), i + 1);
		FString StatId = FString::Printf(TEXT("Stat%d"), i + 1);
		FString DescId = FString::Printf(TEXT("Desc%d"), i + 1);
		Cards[i].Card = Cast<UButton>(GetWidgetFromName(*CardId));
		Cards[i].Card->OnClicked.AddDynamic(this, &UASeedUI_LevelUp::OnCardClicked);
		Cards[i].Stat = Cast<UTextBlock>(GetWidgetFromName(*StatId));
		Cards[i].Desc = Cast<UTextBlock>(GetWidgetFromName(*DescId));
	}
}

void UASeedUI_LevelUp::OnCardClicked()
{
	for (int32 i=0; i<3; i++)
	{
		if (Cards[i].Card->IsHovered() || Cards[i].Card->IsPressed())
		{
			FCard SelectedCard;
			SelectedCard.StatEnum = Cards[i].StatEnum;
			SelectedCard.Value = Cards[i].Value;
			OnCardSelected.Broadcast(SelectedCard);
			break;
		}
	}
}

void UASeedUI_LevelUp::InitializeCards(const TArray<FLevelUpRewardInfo>& ShuffleResults)
{
	for (int32 i=0; i<3; i++)
	{
		Cards[i].Stat->SetText(ShuffleResults[i].StatName);
		
		// Decide Value here
		int32 Value = FMath::RandRange(ShuffleResults[i].MinValue, ShuffleResults[i].MaxValue);
		Cards[i].Value = Value;
		
		FString FullDesc = ShuffleResults[i].Description + FString::FromInt(Value);
		Cards[i].Desc->SetText(FText::FromString(FullDesc));

		Cards[i].StatEnum = ShuffleResults[i].StatEnum;
	}
}
```

<img width="932" height="541" alt="image" src="https://github.com/user-attachments/assets/d0697ab2-2430-4484-a5ad-75bc25e5a55c" />

<br>

### 4. 기타 핵심 구현 컨텐츠
- **`HLSL Outliner Material`**: 커스텀 HLSL 셰이더를 작성하여 적에게 커서를 올릴 시 아웃라이너가 켜지는 기능을 직접 구현했습니다.
- **`BehaviorTree` & `Blackboard AI`**: 거리 계산, 타겟팅, 스턴(Stun) 상태 제어를 포함한 적 AI를 `BehaviourTree`및 `BehaviourTreeTask`와 `Blackboard`를 직접 C++클래스로 제작하여 구현했습니다.
- **`Niagara VFX` & `GameplayCue`**: 나이아가라 이펙트로 플레이어 슬립스트림(주위 속도 슬로우) 스킬 사용 시 시각 효과인 잔상을 제작했습니다. GAS의 `GameplayCue`를 블루프린트로 제작하여 간단히 탄환별 발사/피격 특수 효과를 적용했습니다.

---

## 트러블슈팅 (Troubleshooting)

### 1. 관통 탄환 구현 시 충돌 판정 및 다중 타격 오작동
- **문제**: 기존 탄환은 물리 충돌(`Block`) 시 발생하는 `OnProjectileStop` 이벤트에 의존하여, 적 개체(`Overlap`)를 통과하며 관통 다중 타격을 수행할 수 없는 구조였습니다.
- **해결**:
	- 충돌 프로필을 `PlayerAttack`으로 세분화하고 콜백을 `OnComponentBeginOverlap`으로 재바인딩했습니다.
	- `PierceCount` 카운터를 도입하여, 지정된 관통 횟수를 초과하기 전까지 탄환이 파괴되지 않고 연속 `Overlap` 이벤트를 발생시키도록 변경했습니다.

### 2. GAS Event Data 단일 태그 제약으로 인한 복수 효과(Effect) 적용 제한
- **문제**: 탄환 발사 Ability 수행 시 단일 `FGameplayTag`만 페이로드에 전달되어, "화염 데미지 + 둔화"처럼 한 번의 공격에 여러 `GameplayEffect`를 동시에 입히는 덱빌딩 시너지를 구현할 수 없었습니다.
- **해결**:
	- 발사체 컴포넌트의 데이터 페이로드를 `TArray<FGameplayTag>` 구조로 리팩토링했습니다.
	- 플레이어 총알 충돌 함수 호출 시 태그 배열 전체를 `FGameplayEventData`로 패킹하여 전송함으로써, 단일 탄환으로 유연한 다중 상태이상을 적에게 적용할 수 있도록 확장했습니다.

```c++

FGameplayEventData	EventData;
EventData.Target = Hit.GetActor();
EventData.Instigator = OwnerController->GetPawn();
EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Hit"));
EventData.TargetTags.AddTag(BulletData->GameplayBulletHitCueTag); // 0번 인덱스는 Cue, 1번부터는 Effect Tags

for (const FGameplayTag Tag : BulletData->GameplayEffectTags)
{
	EventData.TargetTags.AddTag(Tag);
}

EventData.EventMagnitude = BulletData->EffectDuration;

FGameplayAbilityTargetData_SingleTargetHit* TargetData =
	new FGameplayAbilityTargetData_SingleTargetHit(Hit);

EventData.TargetData.Add(TargetData);

UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerController->GetPawn(), EventData.EventTag, EventData);
```

---

## 개발 회고 및 성찰

- **GAS 설계 패턴을 직접 체험**:
  `Ability`, `Effect`, `AttributeSet`을 독립적으로 설계하면서 모듈화된 아키텍처의 중요성을 체감했습니다. 코드 간 결합도를 낮추어 기능 추가나 밸런스 수정 시 발생하는 사이드 이펙트를 최소화할 수 있었습니다. 이번 프로젝트는 캐주얼 슈팅게임에 덱빌딩이라는 복잡한 요소를 추가하여 GAS를 활용했지만, 다음에는 데미지, 기술, 상태이상 등 복잡한 정보가 자주 교차하는 RPG 게임에서 GAS 플러그인을 활용하면 수월할 것이라고 생각했습니다. 게임 하나를 오랜 시간 서비스하는 실제 라이브 서비스 RPG들처럼 수정과 확장이 거듭되는 게임일수록 기초 설계가 더 중요할 것이라는 생각을 확실히 가지게 되었습니다.
- **언리얼 생태계에 대한 이해도 고양**:
  초기에는 데이터 테이블 로딩 시점과 GAS 초기화(`InitAbilityActorInfo`) 순서가 꼬여 Attribute 값이 반영되지 않거나, 비동기 데이터 로딩 과정에서 널 참조가 발생하는 이슈가 있었습니다. 게임 시작 단계(`BeginPlay`, `OnConstruction`)에 맞춰 초기화 순서를 정교하게 동기화함으로써 안정적인 생성 흐름을 구축했습니다. 이번 경험을 시작으로 더 높은 수준의 게임 엔진 활용 경험을 쌓아갈 것을 새로운 목표로 삼게 되었습니다. 
