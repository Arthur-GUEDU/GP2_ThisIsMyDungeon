// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/LevelDefinitionActor.h"

#include "Enemies/EnemyManagerInstanceSubsystem.h"

// Sets default values
ALevelDefinitionActor::ALevelDefinitionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALevelDefinitionActor::BeginPlay()
{
	//Checks for if the level definition is complete. Throws errors and warning depending on if the level defintion is just poorly defined or cant work at all, destroys itself if incomplete
	bool bIsValidDefinition = true;
	if(EnemyStoreLocation == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelDefinitionActor: Enemy Store Location is set to default"));
	}
	if(ExitLocation == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelDefinitionActor: Exit Location is still set to default"));
	}
	if(WaveCount==0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelDefinitionActor: Wave Count is set to 0!"));
	}
	if(!EnemyCountCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: Enemy Count Curve not defined!"));
		bIsValidDefinition = false;
	}
	if(EnemyTypes.Num()==0)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: No Enemy Types defined!"));
		bIsValidDefinition = false;
	}
	if(!ObjectiveActor)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: No Objective Actor defined!"));
		bIsValidDefinition = false;
	}
	if(!DropBlueprint)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: No Drop Blueprint defined!"));
		bIsValidDefinition = false;
	}
	if(SpawnPointsPositions.Num()==0)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: No Spawn Point positions defined!"));
		bIsValidDefinition = false;
	}
	if(!bIsValidDefinition)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelDefinitionActor: Level Definition incomplete, deleting definition"));
		GEngine->AddOnScreenDebugMessage(0001, 2.f, FColor::Red, TEXT("Level Definition Incomplete, Errors WILL happen if you continue playing"));
		Destroy();
		return;
	}
	//declares itself to the enemy manager subsystem
	GetGameInstance()->GetSubsystem<UEnemyManagerInstanceSubsystem>()->SetLevelDefinition(this);;
	Super::BeginPlay();
	
}

