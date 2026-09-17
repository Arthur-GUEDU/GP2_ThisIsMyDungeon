// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyManagerInstanceSubsystem.h"


void UEnemyManagerInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::PreLevelRemovedFromWorld.AddUObject(this, &UEnemyManagerInstanceSubsystem::OnLevelChange);
}

void UEnemyManagerInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FWorldDelegates::PreLevelRemovedFromWorld.RemoveAll(this);
}

/**
 * Returns a pointer to an inactive enemy of the specified type. If none is present in the list, creatres a new one.
 */
AEnemyCharacter* UEnemyManagerInstanceSubsystem::GetInstantiatedEnemyOfType(TSubclassOf<AEnemyCharacter> EnemyType)
{
	for (AEnemyCharacter* Enemy : InstantiatedEnemies)
	{
		if(!Enemy->IsActive() && Enemy->IsA(EnemyType))
		{
			return Enemy;
		}
	}
	return InstantiateEnemy(EnemyType);
}

AActor_Drop* UEnemyManagerInstanceSubsystem::GetInstantiatedCoinDrop()
{
	for (AActor_Drop* drop : InstantiatedDrops)
	{
		if(drop->GetActive())
		{
			return drop;
		}
	}
	return InstantiateCoinDrop();
}


bool UEnemyManagerInstanceSubsystem::Spawn()
{
	if(!LevelDefinition)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManagerInstanceSubsystem: No Level Definiton!"));
		return false;
	}
	if(ActiveEnemyCount>=LevelDefinition->MaxActiveEnemies)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Manager: Cant Spawn: Max Enemy Count Reached"));
		return false;
	}
	int value = FMath::RandRange(0,ActiveEnemyTypes.Last().Value-1); 
	for (FEnemyTypeInfo enemyInfo : ActiveEnemyTypes)
	{
		if(enemyInfo.Value>value)
		{
			if(AEnemyCharacter* enemy = GetInstantiatedEnemyOfType(enemyInfo.EnemyType))
			{
				int spawnPointIndex = FMath::RandRange(0,SpawnPoints.Num()-1);
				enemy->Activate(SpawnPoints[spawnPointIndex], SpawnPoints[spawnPointIndex]->GetActorLocation(), SpawnPoints[spawnPointIndex]->GetActorRotation());
				ActiveEnemyCount++;
				return true;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Manager: Failed to spawn enemy"));
	return false;
}

bool UEnemyManagerInstanceSubsystem::SpawnCoinDrop(FVector location, int treasureAmount)
{
	AActor_Drop* drop = GetInstantiatedCoinDrop();
	drop->Activate(location,treasureAmount);
	drop->Objective = LevelDefinition->ObjectiveActor;
	return true;
}

AEnemyCharacter* UEnemyManagerInstanceSubsystem::InstantiateEnemy(TSubclassOf<AEnemyCharacter> EnemyBlueprint)
{
	if(!LevelDefinition)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManagerInstanceSubsystem: No Level Definiton!"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	AEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyBlueprint, LevelDefinition->EnemyStoreLocation, FRotator::ZeroRotator, SpawnParams);
	if (NewEnemy)
	{
		InstantiatedEnemies.Add(NewEnemy);
		return NewEnemy;
	}
	return nullptr;
}

AActor_Drop* UEnemyManagerInstanceSubsystem::InstantiateCoinDrop()
{
	if(!LevelDefinition)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManagerInstanceSubsystem: No Level Definiton!"));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	AActor_Drop* NewEnemy = GetWorld()->SpawnActor<AActor_Drop>(LevelDefinition->DropBlueprint,LevelDefinition->EnemyStoreLocation, FRotator::ZeroRotator, SpawnParams);
	if (NewEnemy)
	{
		InstantiatedDrops.Add(NewEnemy);
		NewEnemy->Objective = LevelDefinition->ObjectiveActor;
		return NewEnemy;
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to spawn drop"))
	return nullptr;
}

void UEnemyManagerInstanceSubsystem::StartWaveSpawning()
{
	CurrentWave = 1;
	EnemiesLeftInWave = LevelDefinition->EnemyCountCurve->GetFloatValue(CurrentWave);
	EnemiesLeftToSpawn = EnemiesLeftInWave;
	spawnDelay = 60.f/EnemiesLeftInWave; //we want to try to spawn every enemy in roughly one minute to make the waves feel quick
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &UEnemyManagerInstanceSubsystem::WaveSpawn, spawnDelay, true);
	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer2, this, &UEnemyManagerInstanceSubsystem::WaveSpawningStart, spawnDelay, false);
	UE_LOG(LogTemp, Display, TEXT("Enemy Manager: Wave Spawning has started"));
}



void UEnemyManagerInstanceSubsystem::AdvanceToNextWave()
{
	GetWorld()->GetTimerManager().ClearTimer(spawnTimer);
	CurrentWave++;
	if(CurrentWave > LevelDefinition->WaveCount)
	{
		LevelEnd();
		return;
	}
	OnWaveEnd.Broadcast();
	EnemiesLeftInWave = LevelDefinition->EnemyCountCurve->GetFloatValue(CurrentWave);
	EnemiesLeftToSpawn = EnemiesLeftInWave;
	spawnDelay = LevelDefinition->WaveDuration/EnemiesLeftInWave;
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &UEnemyManagerInstanceSubsystem::WaveSpawn, spawnDelay, true);
	PauseWaveSpawning();
	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &UEnemyManagerInstanceSubsystem::ResumeWaveSpawning, LevelDefinition->WaveDelay, false);
	for (int a = 0; a < EnemyTypes.Num(); ++a) //we add the combined value of all the previous elements to each element in the list so that the values in the EnemyDef becomes a sort of "step", for example the values [1,2,1] from three types become [1,3,4]. We use this for the Spawn() function;
	{
		if(EnemyTypes[a].WaveStartSpawn==CurrentWave)
		{
			int tempValue = ActiveEnemyTypes.Last().Value;
			ActiveEnemyTypes.Add(EnemyTypes[a]);
			ActiveEnemyTypes[ActiveEnemyTypes.Num()-1].Value+=tempValue;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Enemy Manager: Wave #%i"), CurrentWave);
}

void UEnemyManagerInstanceSubsystem::WaveSpawn()
{
	if(EnemiesLeftToSpawn!=0)
	{
		if(Spawn())
			EnemiesLeftToSpawn--;
		//Update the values for the new wave, 
		if(EnemiesLeftToSpawn <= 0)
		{
			PauseWaveSpawning();
		}
	}
}

void UEnemyManagerInstanceSubsystem::WaveSpawningStart()
{
	OnWaveStart.Broadcast();
}

void UEnemyManagerInstanceSubsystem::OnLevelChange(ULevel* level, UWorld* LoadedWorld)
{
	GetWorld()->GetTimerManager().ClearTimer(spawnTimer);
	GetWorld()->GetTimerManager().ClearTimer(waveDelayTimer);
	SpawnPoints.Empty();
	InstantiatedEnemies.Empty();
	EnemyTypes.Empty();
	ActiveEnemyTypes.Empty();
	InstantiatedDrops.Empty();
	LevelDefinition = nullptr;
}

void UEnemyManagerInstanceSubsystem::LevelEnd()
{
	OnLevelEnd.Broadcast();
}

/**
 * after a level definiton declares itself, sets up the level by instantating the declared spawnPoints and getting the inactive enemy store location
 * @param _LevelDefinition a pointer to the level definition trying to declare itself
 */
void UEnemyManagerInstanceSubsystem::SetLevelDefinition(ALevelDefinitionActor* _LevelDefinition)
{
	
	LevelDefinition =_LevelDefinition;
	EnemyStoreLocation = _LevelDefinition->EnemyStoreLocation;
	EnemyTypes = _LevelDefinition->EnemyTypes;
	int Value = 0;
	int tempvalue;
	for (int a = 0; a < EnemyTypes.Num(); ++a) //we add the combined value of all the previous elements to each element in the list so that the values in the EnemyDef becomes a sort of "step", for example the values [1,2,1] from three types become [1,3,4]. We use this for the Spawn() function;
	{
		if(EnemyTypes[a].WaveStartSpawn==0)
			ActiveEnemyTypes.Add(EnemyTypes[a]);
	}
	for (int a = 0; a < ActiveEnemyTypes.Num(); ++a)
	{
		tempvalue = ActiveEnemyTypes[a].Value;
		ActiveEnemyTypes[a].Value += Value;
		Value += tempvalue;
	}
	for(FVector location: _LevelDefinition->SpawnPointsPositions) //spawn the specified spawn positions in the level and store a reference to them
	{
		FActorSpawnParameters SpawnParams;
		if(ASpawnerActor* spawn = GetWorld()->SpawnActor<ASpawnerActor>(location, FRotator::ZeroRotator, SpawnParams))
		{
			SpawnPoints.Add(spawn);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &UEnemyManagerInstanceSubsystem::StartWaveSpawning, LevelDefinition->LevelStartDelay, false);
}

AEnemyCharacter* UEnemyManagerInstanceSubsystem::GetNearestEnemyInRange(FVector location, float range)
{
	AEnemyCharacter* nearestEnemy = nullptr; //so that it returns nullptr if no enemy found
	float previousEnemyDistance = 99999999999999.f; //ridiculously high value so the bCloserThanCurrentClosest is true on first try
	for (AEnemyCharacter* enemy : InstantiatedEnemies)
	{
		if(!enemy->IsActive()) //we do not want to consider enemies that are inactive, aka uninstantiated
			continue;
		float distance = FVector::Distance(enemy->GetActorLocation(), location);
		bool bInRange = distance<=range;
		bool bCloserThanCurrentClosest = distance<previousEnemyDistance;
		if((!nearestEnemy || bInRange) && bCloserThanCurrentClosest)
		{
			nearestEnemy = enemy;
			previousEnemyDistance = distance;
		}
	}

	return nearestEnemy;
}

TArray<AEnemyCharacter*> UEnemyManagerInstanceSubsystem::GetEnemiesInRange(FVector location, float range)
{
	TArray<AEnemyCharacter*> enemiesInRange;
	for (AEnemyCharacter* enemy : InstantiatedEnemies)
	{
		if(!enemy->IsActive()) //we do not want to consider enemies that are inactive, aka uninstantiated
			continue;
		float distance = FVector::Distance(enemy->GetActorLocation(), location);
		if(distance<=range && distance != 0)
		{
			enemiesInRange.Add(enemy);
		}
	}
	return enemiesInRange;
}


void UEnemyManagerInstanceSubsystem::PauseWaveSpawning()
{
	GetWorld()->GetTimerManager().PauseTimer(spawnTimer);
}

void UEnemyManagerInstanceSubsystem::ResumeWaveSpawning()
{
	OnWaveStart.Broadcast();
	GetWorld()->GetTimerManager().UnPauseTimer(spawnTimer);
}

void UEnemyManagerInstanceSubsystem::OnEnemyDeactivate()
{
	ActiveEnemyCount--;
	EnemiesLeftInWave--;
	if(EnemiesLeftInWave <= 0)
	{
		AdvanceToNextWave();
	}
}


