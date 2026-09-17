// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/ObjectiveActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectiveActor::AObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
}

float AObjectiveActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
		return 0.f;

	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	if(TreasureTakenCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TreasureTakenCue, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Sound Cue!"))
	}
	if (Health <= 0.f)
	{
		ObjectiveDeath();
		return 0.f;
	}

	OnObjectiveHit.Broadcast();

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AObjectiveActor::AddTreasure(int amount)
{
	Health += amount;
	OnObjectiveHit.Broadcast();

	if(Health > MaxHealth)
		Health = MaxHealth;
}

void AObjectiveActor::ObjectiveDeath()
{
	bIsDead = true;
	OnObjectiveDeath.Broadcast();

	return;
}

