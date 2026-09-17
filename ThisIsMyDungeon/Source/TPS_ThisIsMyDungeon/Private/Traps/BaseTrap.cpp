#include "Traps/BaseTrap.h"

// Sets default values
ABaseTrap::ABaseTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseTrap::BeginPlay()
{
	Super::BeginPlay();
	Deactivate();
}

void ABaseTrap::Activate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsActive = true;
}

void ABaseTrap::Deactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bIsActive = false;
}

void ABaseTrap::Spawn(FVector _Spawnpoint, FRotator _Rotation)
{
	SetActorLocation(_Spawnpoint);
	SetActorRotation(_Rotation);
	Activate();
}
