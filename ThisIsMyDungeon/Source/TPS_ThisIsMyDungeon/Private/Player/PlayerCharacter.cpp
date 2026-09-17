// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Traps/TrapEdition.h"
#include "Player/PlayerProjectile.h"

// Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Inputs
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Character movement
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Timer
#include "Timer/Timer.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetupPlayerCamera();
	ParrotLaunchPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Parrot Launch Position"));
	ParrotLaunchPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCDTimer = NewObject<UTimer>(this);
	MoneyGainTimer = NewObject<UTimer>(this);

	MoneyGainTimer->OnTimerEnd.BindUFunction(this, FName("GainMoneyTime"));
	MoneyGainTimer->SetupTimerAndStart(MoneyGainTime, MoneyGainTimeStamp);

	SpawnPosition = GetActorLocation();
	OnPlayerRespawn.Broadcast();
}

bool APlayerCharacter::TryToSpendMoney(int MoneyAmount)
{
	if (MoneyAmount > Money)
		return false;

	Money -= MoneyAmount;

	Money = FMath::Clamp(Money, 0, MaxMoney);

	OnPlayerMoneyUpdated.Broadcast();
	TrapEdition->CheckTrapCost(Money);

	return true;
}

void APlayerCharacter::OnTrapPlaced_Implementation(bool IsTrapPlaced)
{}

void APlayerCharacter::GainMoney(int MoneyAmount)
{
	Money += MoneyAmount;
	Money = FMath::Clamp(Money, 0, MaxMoney);

	OnPlayerMoneyUpdated.Broadcast();
	TrapEdition->CheckTrapCost(Money);
}

void APlayerCharacter::RespawnPlayer()
{
	Health = MaxHealth;
	bIsDead = false;

	SetActorLocation(SpawnPosition);

	APlayerController* playerController = Cast<APlayerController>(Controller);

	OnPlayerRespawn.Broadcast();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
		input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
		input->BindAction(ShootInputAction, ETriggerEvent::Started, this, &APlayerCharacter::ShootAction);
		input->BindAction(SwitchModeInputAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchModeAction);
		input->BindAction(TrapUINavInputAction, ETriggerEvent::Started, this, &APlayerCharacter::TrapUINavAction);
		input->BindAction(ConfirmTrapInputAction, ETriggerEvent::Started, this, &APlayerCharacter::ConfirmTrapAction);
		input->BindAction(RotateTrapInputAction, ETriggerEvent::Started, this, &APlayerCharacter::RotateTrapAction);
		input->BindAction(PauseInputAction, ETriggerEvent::Started, this, &APlayerCharacter::PauseAction);
	}
}

void APlayerCharacter::SetupPlayerCamera()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	FVector2D movementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), movementVector.X);
	AddMovementInput(GetActorRightVector(), movementVector.Y);

	if (bIsEditMode)
		CheckTrapPosition();
}

void APlayerCharacter::LookAction(const FInputActionValue& Value)
{
	FVector2D lookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(lookAxis.X);
	AddControllerPitchInput(lookAxis.Y);

	if (bIsEditMode)
		CheckTrapPosition();
}

void APlayerCharacter::ShootAction(const FInputActionValue& Value)
{
	if (!Projectile)
		return;

	if (bIsEditMode || ProjectileCDTimer->CountdownTime > 0.f)
		return;

	FVector launchPosition = ParrotLaunchPosition->GetComponentLocation();

	// Determines the launch direction

	FVector launchDirection = Camera->GetForwardVector() * 1000000.f;
	// Multiply to a high trash value to get a point really far away

	launchDirection += Camera->GetComponentLocation();
	launchDirection -= launchPosition;
	launchDirection.Normalize();

	Projectile->LaunchProjectile(launchPosition, launchDirection);
}

void APlayerCharacter::SwitchModeAction(const FInputActionValue& Value)
{
	if (bIsEditMode)
	{
		bIsEditMode = false;
		TrapEdition->Disable();
	}
	else
	{
		bIsEditMode = true;
		TrapEdition->Enable();
	}
}

void APlayerCharacter::TrapUINavAction(const FInputActionValue& Value)
{
	if (!bIsEditMode)
		return;

	float inputValue = Value.Get<float>();

	// Goes from the first trap to the last one in the list
	if (inputValue < 0.f && CurrentTrapIndex == 0)
	{
		CurrentTrapIndex = TrapEdition->GetTrapTypes().Num() - 1;
		TrapEdition->SwitchTrapType(CurrentTrapIndex);
		TrapEdition->CheckTrapCost(Money);

		return;
	}

	// Goes from the last trap to the first one in the list
	if (inputValue > 0.f && CurrentTrapIndex == TrapEdition->GetTrapTypes().Num() - 1)
	{
		CurrentTrapIndex = 0;
		TrapEdition->SwitchTrapType(CurrentTrapIndex);
		TrapEdition->CheckTrapCost(Money);

		return;
	}

	// Increments or decrements the index of the trap
	CurrentTrapIndex += FMath::FloorToInt(inputValue);
	TrapEdition->SwitchTrapType(CurrentTrapIndex);
	TrapEdition->CheckTrapCost(Money);

	return;
}

void APlayerCharacter::ConfirmTrapAction(const FInputActionValue& Value) 
{
	if (!bIsEditMode)
		return;

	TryPlaceTrap();
}

void APlayerCharacter::RotateTrapAction(const FInputActionValue& Value)
{
	if (!bIsEditMode)
		return;

	TrapEdition->RotateTrap();
}

void APlayerCharacter::PauseAction(const FInputActionValue& Value)
{
}

void APlayerCharacter::CheckTrapPosition()
{
	FHitResult hit;

	FVector traceStart = Camera->GetComponentLocation();
	FVector traceEnd = traceStart + Camera->GetForwardVector() * 1000.0f;

	FCollisionQueryParams queryParams;

	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, Channel, queryParams);
	
	if (hit.bBlockingHit && hit.Distance < MaxEditDistance)
	{
		TrapEdition->UpdateLocation(FVector3f(hit.ImpactPoint));
	}
	else
	{
		traceStart = GetTransform().GetLocation();
		GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceStart + FVector(0, 0, -1) * 1000, ECollisionChannel::ECC_Visibility, queryParams);
		TrapEdition->UpdateLocation(FVector3f(hit.ImpactPoint));
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInvestigator, AActor* DamageCauser)
{
	if (bIsDead)
		return Health;

	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		PlayerDeath();
		return Health;
	}

	OnPlayerHit.Broadcast();

	return Health;
}

void APlayerCharacter::PlayerDeath()
{
	bIsDead = true;

	APlayerController* playerController = Cast<APlayerController>(Controller);

	OnPlayerDeath.Broadcast();
}

void APlayerCharacter::GainMoneyTime()
{
	GainMoney(MoneyGain);
	MoneyGainTimer->SetupTimerAndStart(MoneyGainTime, MoneyGainTimeStamp);
}

void APlayerCharacter::TryPlaceTrap()
{
	FTrapTypeInfo TrapInfo = TrapEdition->GetCurrentTrapInfo();

	if (TrapEdition->CanPlace())
	{
		float Cost = TrapInfo.TrapCost;
		if (TryToSpendMoney(Cost))
		{
			TrapEdition->SpawnTrap();
			TrapEdition->CheckTrapCost(Money);
			OnTrapPlaced(true);

			return;
		}
	}

	OnTrapPlaced(false);
}
