// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/SpawnerActor.h"

#include "Enemies/EnemyManagerInstanceSubsystem.h"

// Sets default values
ASpawnerActor::ASpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
}

/**
 * Spawns an enemy on the spawn point by Getting an instantiated enemy from the enemy manager then instantiating it 
 * @param EnemyType the type of enemy we want spawned
 */
void ASpawnerActor::Spawn(const TSubclassOf<AEnemyCharacter> EnemyType)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = FRotator::ZeroRotator;

		if(AEnemyCharacter* cont = EnemyManagerSubSystem->GetInstantiatedEnemyOfType(EnemyType))
			cont->Activate(this, GetActorLocation(), GetActorRotation());
		else
			UE_LOG(LogTemp, Warning, TEXT("%s : failed to instantiate enemy"), *GetName());

}

// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	//get a reference to the subsystem
	EnemyManagerSubSystem = GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>();
	
	Super::BeginPlay();
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

