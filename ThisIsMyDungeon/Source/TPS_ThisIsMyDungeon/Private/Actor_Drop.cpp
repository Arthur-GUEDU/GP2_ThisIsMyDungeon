// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Drop.h"

#include "Enemies/EnemyManagerInstanceSubsystem.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AActor_Drop::AActor_Drop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

bool AActor_Drop::GetActive()
{
	return bIsActive;
}

void AActor_Drop::Activate(FVector Location, int treasureAmount)
{
	if(!bIsActive)
	{
		//SetActorScale3D(FVector::One()*(treasureAmount/150.f)); //we make the actor scale depending on how much treasure there is
		GetWorld()->GetTimerManager().SetTimer(despawnTimer, this, &AActor_Drop::Deactivate, 5.f, false);
		SetActorLocation(Location);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		TreasureAmount=treasureAmount/2; //we only want to return half the enemy's treasure to the treasure pile
		bIsActive = true;
	}
}

void AActor_Drop::Deactivate()
{
	if(bIsActive)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		TreasureAmount=0;
		bIsActive = false;
	}
}

void AActor_Drop::BeginPlay()
{
	Super::BeginPlay();
}





