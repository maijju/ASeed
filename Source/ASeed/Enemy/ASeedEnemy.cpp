// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemy.h"

#include "../Ability/ASeedGA_Hit.h"
#include "../Ability/ASeedGA_EnemyFire.h"
#include "../UI/ASeedUI_HPBar.h"

#include "ASeedEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "../ASeedGameMode.h"

AASeedEnemy::AASeedEnemy()
{
	/*--------------DATA--------------*/
	DataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Enemy/DT_EnemyData.DT_EnemyData'"));
	BulletDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Enemy/DT_EnemyBulletData.DT_EnemyBulletData'"));

	/*--------------BODY--------------*/
	Body->SetCanEverAffectNavigation(false);

	/*--------------MESH--------------*/
	MeshComp->SetRelativeRotation(FRotator(0, -90, 0));

	/*--------------MOVEMENT--------------*/
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	/*--------------AI--------------*/
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AASeedEnemyController::StaticClass();
	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/AI/BT_Enemy.BT_Enemy'"));
	if (BTAsset.Succeeded())
		BehaviorTree = BTAsset.Object;

	/*--------------UI--------------*/
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(MeshComp);

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBar.Succeeded())
	{
		WidgetComp->SetWidgetClass(HPBar.Class);
	}

	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetDrawSize(FVector2D(100, 10));
	WidgetComp->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> WidgetMtrl(TEXT("/Script/Engine.Material'/Game/UI/Material/MT_WidgetComponent_World.MT_WidgetComponent_World'"));

	if (WidgetMtrl.Succeeded())
	{
		WidgetComp->SetMaterial(0, WidgetMtrl.Object);
	}

	WidgetComp->SetAbsolute(false, true, false);

	/*--------------GAS--------------*/
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UASeedEnemyAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UASeedEnemyAttributeSet>(AttributeSet);
}

void AASeedEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*--------------UI--------------*/
	HPBarWidget = Cast<UASeedUI_HPBar>(WidgetComp->GetWidget());

	/*--------------GAS--------------*/
	ASC->InitAbilityActorInfo(this, this);
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_Hit::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_EnemyFire::StaticClass(), 1, INDEX_NONE, this));
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Custom.State.Stun")),
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AASeedEnemy::OnGameplayStun);

}

void AASeedEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AASeedEnemy::InitializeEnemy(FName EnemyKey)
{
	/*--------------LOAD DATA--------------*/
	Data = DataRef.LoadSynchronous();
	FEnemyData* Row = Data->FindRow<FEnemyData>(EnemyKey, TEXT(""));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *EnemyKey.ToString());
		return;
	}

	bHasProjectile = Row->bHasProjectile;
	BulletKey = Row->BulletKey;

	if (bHasProjectile)
	{
		BulletData = BulletDataRef.LoadSynchronous();
		BulletRow = BulletData->FindRow<FEnemyBulletData>(BulletKey, TEXT(""));
		if (!BulletRow)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *BulletKey.ToString());
			return;
		}
	}

	/*--------------APPLYING MESH & ABP--------------*/
	MeshComp->SetSkeletalMesh(Row->Mesh);
	MeshComp->SetAnimInstanceClass(Row->ABP);
	AnimInst = Cast<UASeedEnemyAnimInst>(MeshComp->GetAnimInstance());

	FBoxSphereBounds Bounds = MeshComp->Bounds;
	float Height = Bounds.BoxExtent.Z * 2.0f;
	float Radius = FMath::Max(Bounds.BoxExtent.X, Bounds.BoxExtent.Y);

	Body->SetCapsuleHalfHeight(Height / 2);
	Body->SetCapsuleRadius(Radius);
	Body->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	MeshComp->SetRelativeLocation(FVector(0.0, 0.0, -Height/2));
	WidgetComp->SetRelativeLocation(FVector(0, 0, Height + 30));

	/*--------------SPOTLIGHT--------------*/
	USpotLightComponent* SpotLight = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
	if (SpotLight)
	{
		SpotLight->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
		SpotLight->RegisterComponent();

		SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, Height*0.8f));
		SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

		SpotLight->SetLightColor(FColor(255, 100, 100));
		SpotLight->SetIntensity(1000.0f);
		SpotLight->SetAttenuationRadius(100.f);
		SpotLight->ShadowResolutionScale = 0.5f;

		SpotLight->SetInnerConeAngle(15.0f);
		SpotLight->SetOuterConeAngle(40.0f);
	}

	/*--------------ASSIGN PROPERTIES--------------*/
	Name = Row->Name;
	bIsBossEnemy = Row->bIsBossEnemy;
	AttackAbilityTag = Row->AttackAbilityTag;
	AttackEffectTags = Row->AttackEffectTags;
	AttackCueTag = Row->AttackCueTag;
	AttributeSet->SetHP(Row->HP);
	AttributeSet->SetHPMax(Row->HPMax);
	AttributeSet->SetAttack(Row->Attack);
	AttributeSet->SetAttackRange(Row->AttackRange);
	AttributeSet->SetAttackSpeed(Row->AttackSpeed);
	AttributeSet->SetMoveSpeed(Row->MoveSpeed);
	AnimInst->InitSeqMap(Row->SeqMap);
	ExpReward = Row->Exp;
	CreditReward = Row->Gold;

	/*--------------UI--------------*/
	HPBarWidget->SetHPPercent(AttributeSet->GetHP(), AttributeSet->GetHPMax());

	/*--------------RUN AI--------------*/
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Target is always Player
	AAIController* AIController = Cast<AAIController>(GetController());

	if (Player && AIController && BehaviorTree)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Player);
		AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), Row->AttackRange);
		MovementComp->MaxSpeed = Row->MoveSpeed;
	}
}

void AASeedEnemy::Attack()
{
	if (ASC)
	{
		FVector	Forward = GetActorForwardVector();

		FVector HitStart = GetActorLocation() + Forward * 50.f;
		FVector HitEnd = GetActorLocation() + Forward * (50.f + AttributeSet->GetAttackRange());

		float Radius = 300.f;

		FHitResult HitResult;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = false;

		FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			HitStart,
			HitEnd,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6,
			Sphere,
			Params
		);

		FGameplayEventData	EventData;
		AAIController* AIController = Cast<AAIController>(GetController());
		EventData.Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));
		EventData.Instigator = this;
		EventData.EventTag = AttackAbilityTag;

		/*--------------------------------------------------*/
		/*----------------HARD CODE WARNING-----------------*/
		/*----------INDEX 0 IS ONLY USE AS CUE--------------*/
		/*----------INDEX 1 ~ IS ONLY USE AS EFFECT---------*/
		/*--------------------------------------------------*/
		EventData.TargetTags.AddTag(AttackCueTag);

		for (const FGameplayTag Tag : AttackEffectTags)
		{
			EventData.TargetTags.AddTag(Tag);
		}

		FGameplayAbilityTargetData_SingleTargetHit* TargetData =
			new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

		EventData.TargetData.Add(TargetData);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
	}
}

void AASeedEnemy::Fire()
{
	if (ASC)
	{
		TObjectPtr<UASeedEnemyBulletData> BulletPayload = NewObject<UASeedEnemyBulletData>();
		BulletPayload->Location = MeshComp->GetSocketLocation(FName("Muzzle"));
		BulletPayload->GameplayEffectTags = BulletRow->GameplayEffectTags;
		BulletPayload->GameplayMuzzleFlashCueTag = BulletRow->GameplayMuzzleFlashCueTag;
		BulletPayload->GameplayBulletHitCueTag = BulletRow->GameplayBulletHitCueTag;
		BulletPayload->Mesh = BulletRow->Mesh;
		BulletPayload->TrailEffect = BulletRow->TrailEffect;
		BulletPayload->EffectDuration = BulletRow->EffectDuration;
		BulletPayload->BulletSpeed = BulletRow->BulletSpeed;

		FGameplayEventData EventData;
		EventData.Instigator = Owner;
		EventData.OptionalObject = BulletPayload;
		ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(TEXT("Custom.Enemy.Fire")), &EventData);
	}
}

void AASeedEnemy::OnDamage()
{
	if (!bIsBossEnemy)
	{
		HPBarWidget->SetHPPercent(AttributeSet->GetHP(), AttributeSet->GetHPMax());
	}
	else
	{
		AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
		GM->SetBossName(FText::FromString(Name));
		GM->SetBossHPPercent(AttributeSet->GetHP(), AttributeSet->GetHPMax());
	}
}

void AASeedEnemy::OnGameplayStun(const FGameplayTag Tag, int32 Count)
{
	AAIController* AICon = Cast<AAIController>(GetController());
	if (Count > 0)
	{
		AICon->StopMovement();
		AICon->BrainComponent->StopLogic(TEXT("Stun"));
		AnimInst->SetAnimState(EEnemyAnimState::Stun);
	}

	else
	{
		AICon->BrainComponent->RestartLogic();
		AnimInst->SetAnimState(EEnemyAnimState::Idle);
	}
}

void AASeedEnemy::Die()
{
	AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
	GM->EarnEliminationRewards(FRewards(ExpReward, CreditReward));
	Destroy();
}
