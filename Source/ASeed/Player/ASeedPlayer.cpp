// Copyright Epic Games, Inc. All Rights Reserved.

#include "ASeedPlayer.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "../Ability/ASeedGA_Hit.h"
#include "../Ability/ASeedGA_PlayerFire.h"
#include "../Ability/ASeedGA_PlayerReload.h"

#include "../UI/HUD/ASeedUI_PlayerHUD.h"

#include "../ASeedGameMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

AASeedPlayer::AASeedPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	/*--------------CAPSULE(ROOT)--------------*/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	SetRootComponent(GetCapsuleComponent());

	/*--------------MOVEMENT--------------*/
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/*--------------CAMERA--------------*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/*--------------GAS--------------*/
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UASeedPlayerAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UASeedPlayerAttributeSet>(AttributeSet);

	/*--------------SKILL--------------*/
	SkillComp = CreateDefaultSubobject<UASeedPlayerSkillComponent>(TEXT("SkillComponent"));

	/*--------------PROJECTILE--------------*/
	ProjComp = CreateDefaultSubobject<UASeedPlayerProjectileComponent>(TEXT("ProjectileComponent"));
}

void AASeedPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AASeedPlayer::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AASeedPlayer::Attack);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AASeedPlayer::Reload);
		EnhancedInputComponent->BindAction(SkillAAction, ETriggerEvent::Started, this, &AASeedPlayer::SkillA);
		EnhancedInputComponent->BindAction(SkillBAction, ETriggerEvent::Started, this, &AASeedPlayer::SkillB);
		EnhancedInputComponent->BindAction(InstallModuleAction, ETriggerEvent::Started, this, &AASeedPlayer::InstallModule);
	}
}

void AASeedPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	/*--------------ANIMINST--------------*/
	AnimInst = Cast<UASeedPlayerAnimInst>(GetMesh()->GetAnimInstance());

	/*--------------PROJECTILE COMPONENT--------------*/
	ProjComp->UpdateBulletDataByKey(BulletKey);

	/*--------------SKILL COMPONENT--------------*/
	SkillComp->UpdateSkillADataByKey(SkillAKey);
	SkillComp->UpdateSkillBDataByKey(SkillBKey);

	/*--------------GAS--------------*/
	AttributeSet->SetAttack(PlayerData.Attack);
	AttributeSet->SetAmmo(PlayerData.Ammo);
	AttributeSet->SetAmmoMax(PlayerData.AmmoMax);
	AttributeSet->SetDefense(PlayerData.Defense);
	AttributeSet->SetHP(PlayerData.HP);
	AttributeSet->SetHPMax(PlayerData.HPMax);
	AttributeSet->SetAttackRange(PlayerData.AttackRange);
	AttributeSet->SetAttackSpeed(PlayerData.AttackSpeed);
	AttributeSet->SetMoveSpeed(PlayerData.MoveSpeed);
	AttributeSet->SetCooldownReduce(PlayerData.CooldownReduce);
	AttributeSet->SetExpBonus(PlayerData.ExpBonus);
	AttributeSet->SetExpBonus(PlayerData.CoreBonus);

	ASC->InitAbilityActorInfo(this, this);

	OnPlayerHit.Broadcast(AttributeSet->GetHP(), AttributeSet->GetHPMax());
	
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_Hit::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerFire::StaticClass(), 1, INDEX_NONE, this));
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_PlayerReload::StaticClass(), 1, INDEX_NONE, this));
	//ASC->GetGameplayAttributeValueChangeDelegate(UASeedPlayerAttributeSet::GetAttackAttribute()).AddUObject(this, &UASeedPlayerAttributeSet::AttackAttributeChangeDelegate);

	// 특정 태그가 발생되었을 때 호출해줄 함수를 지정할 수 있다.
	// 태그가 제거될 때도 들어온다.
	//ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("Custom.State.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AASeedPlayer::OnGameplayStun);

}

void AASeedPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*--------------DETECT SETUP--------------*/
	FVector WorldLocation, WorldDirection;

	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FHitResult Hit;
	FVector TraceEnd = WorldLocation + WorldDirection * 10000.f;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEnd, ECC_GameTraceChannel5)) // TC5 is Detect TC
		return;

	/*--------------ENABLE TARGET OUTLINE--------------*/
	AASeedTargetPawn* Target = Cast<AASeedTargetPawn>(Hit.GetActor());
	if (CachedTarget)
	{
		if (CachedTarget != Target)
		{
			CachedTarget->EnableOutline(false);
		}
	}
	if (Target)
	{
		Target->EnableOutline(true);
	}
	CachedTarget = Target;

	/*--------------PLAYER ROTATE(LOOK MOUSE OR TARGET)--------------*/
	FVector TargetLocation = Hit.Location;
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(FRotator(0.f, LookAt.Yaw, 0.f));
}

void AASeedPlayer::Move(const FInputActionValue& Value)
{
	CurrentKeyVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, CurrentKeyVector.Y);
		AddMovementInput(RightDirection, CurrentKeyVector.X);
	}
}
	
void AASeedPlayer::Attack()
{
	if (AttributeSet->GetAmmo() > 0.0f)
		AnimInst->PlayMontageByType(EMontageType::Attack, AttributeSet->GetAttackSpeed());
	else
		Reload();
}

void AASeedPlayer::Reload()
{
	if (AttributeSet->GetAmmo() == AttributeSet->GetAmmoMax())
		return;
	// Reload Speed is also affected by Attack Speed
	AnimInst->PlayMontageByType(EMontageType::Reload, AttributeSet->GetAttackSpeed());
}

void AASeedPlayer::InstallModule()
{
	if (AnimInst->IsAnyMontagePlaying())
		return;
	AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
	GM->TryInstallModule();
}

void AASeedPlayer::Fire(FName SocketName)
{
	ProjComp->TryFireBullet(SocketName);
}

void AASeedPlayer::Reloaded()
{
	ASC->TryActivateAbilityByClass(UASeedGA_PlayerReload::StaticClass());
}

void AASeedPlayer::SkillA()
{
	SkillComp->TryActivateSkill(true);
}

void AASeedPlayer::SkillB()
{
	SkillComp->TryActivateSkill(false);
}

void AASeedPlayer::OnDamage(bool IsDead)
{
	OnPlayerHit.Broadcast(AttributeSet->GetHP(), AttributeSet->GetHPMax());

	if (IsDead)
	{
		AnimInst->PlayMontageByType(EMontageType::Die);
	}
}

void AASeedPlayer::OnHPMaxChanged()
{
	OnPlayerHit.Broadcast(AttributeSet->GetHP(), AttributeSet->GetHPMax());
}

