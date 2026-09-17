// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Player/PlayerCharacter.h"
#include "Enemies/EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "Timer/Timer.h"

APlayerProjectile::APlayerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SetRootComponent(SphereComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	SphereComponent->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnHit);
}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	SpawnPosition = GetActorLocation();
	
	ProjectileResetTimer = NewObject<UTimer>(this);
	
	ProjectileResetTimer->OnTimerEnd.BindUFunction(this, FName("DeactivateProjectile"));


	
	DeactivateProjectile();
}

void APlayerProjectile::ActivateProjectile(FVector Position, FVector Direction)
{
	SetActorLocation(Position);
	OnProjectileFired.Broadcast();

	bIsActive = true;

	ProjectileResetTimer->SetupTimerAndStart(ProjectileResetTime, ProjectileResetTimeStamp);

	RotatingMovementComponent->Activate();
	ProjectileMovementComponent->Activate();

	ProjectileMovementComponent->SetVelocityInLocalSpace(ProjectileMovementComponent->InitialSpeed * Direction);

	FRotator randomRotation(FMath::RandRange(-360.f, 360.f), FMath::RandRange(-360.f, 360.f), FMath::RandRange(-360.f, 360.f));
	RotatingMovementComponent->RotationRate = randomRotation;
}

void APlayerProjectile::DeactivateProjectile()
{
	OnProjectileDestroyed.Broadcast();

	bIsActive = false;

	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ZeroVector);
	RotatingMovementComponent->RotationRate = FRotator::ZeroRotator;

	SetActorLocation(SpawnPosition);
	SetActorRotation(FRotator::ZeroRotator);

	RotatingMovementComponent->Deactivate();
	ProjectileMovementComponent->Deactivate();
}

void APlayerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileResetTimer->StopTimer();
	DeactivateProjectile();

	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor);

	if (!enemy)
		return;

	OnProjectileHit.Broadcast();
	UGameplayStatics::ApplyDamage(enemy, DamageAmount, nullptr, this, nullptr);
}

bool APlayerProjectile::LaunchProjectile(FVector LaunchPosition, FVector LaunchDirection)
{
	if (bIsActive)
		return false;

	ActivateProjectile(LaunchPosition, LaunchDirection);
	OnProjectileFired.Broadcast();
	return true;
}