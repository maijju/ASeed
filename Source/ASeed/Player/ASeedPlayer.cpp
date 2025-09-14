// Copyright Epic Games, Inc. All Rights Reserved.

#include "ASeedPlayer.h"
#include "../Enemy/ASeedTargetPawn.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AASeedPlayer::AASeedPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	SetRootComponent(GetCapsuleComponent());

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);
	ParticleComp->bAutoActivate = false;
}

void AASeedPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AASeedPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AASeedPlayer::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AASeedPlayer::Attack);
	}
}

void AASeedPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<UASeedPlayerAnimInst>(GetMesh()->GetAnimInstance());
}

void AASeedPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*--------------Detect--------------*/
	FVector WorldLocation, WorldDirection;
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult Hit;
	FVector TraceEnd = WorldLocation + WorldDirection * 10000.f;
	bool Collision = GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEnd, ECC_GameTraceChannel5);

	if (!Collision)
		return;

	/*--------------Target Highlight--------------*/
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

	/*--------------Look Under Cursor or Target--------------*/
	FVector TargetLocation = Hit.Location;
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(FRotator(0.f, LookAt.Yaw, 0.f));
}

void AASeedPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
	
void AASeedPlayer::Attack()
{
	AnimInst->PlayMontageByType(EMontageType::Attack);
}

void AASeedPlayer::Fire(FName SocketName)
{
	FVector	MuzzleLoctaion = GetMesh()->GetSocketLocation(SocketName);

	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireEffect,
			MuzzleLoctaion,
			GetActorRotation(),
			FVector(2.0f)
		);
	}

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AASeedTestBullet* Bullet = GetWorld()->SpawnActor<AASeedTestBullet>(MuzzleLoctaion, GetActorRotation(), Param);
}
