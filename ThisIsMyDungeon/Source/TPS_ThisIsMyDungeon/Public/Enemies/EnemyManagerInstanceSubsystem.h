// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor_Drop.h"
#include "Level/LevelDefinitionActor.h"
#include "Environment/SpawnerActor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyManagerInstanceSubsystem.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEvent);

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UEnemyManagerInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	
	/**
	* A function that returns a pointer to an instantiated enemy of the specified type.
	* If no uninstantiated enemies of the specified type exist, a new one is created and added to the list
	* @param EnemyType the type of enemy we want to get
	* @return returns a pointer to the instantiated enemy
	*/
	AEnemyCharacter* GetInstantiatedEnemyOfType(TSubclassOf<AEnemyCharacter> EnemyType);
	AActor_Drop* GetInstantiatedCoinDrop();
	UFUNCTION(BlueprintCallable)
	/**
	* Spawns a random enemy of the specified types at a random spawn points. Spawn chance is picked based on the enemies value: the higher the value, the higher the chance of spawning.
	*/
	bool Spawn();
	//Same  as Spawn() but for a coin, dropped by enemies if theyre carrying a treasure
	bool SpawnCoinDrop(FVector location, int treasureAmount);

	
	void SetLevelDefinition(ALevelDefinitionActor* _LevelDefinition);
	
	//returns the location where we store inactive enemies
	UFUNCTION(BlueprintCallable) FVector GetInactiveLocation() { return EnemyStoreLocation; };
	
	//finds the nearest active enemy to a given location and returns it
	UFUNCTION(BlueprintCallable) AEnemyCharacter* GetNearestEnemyInRange(FVector location, float range);
	//returns all active enemies in range to a given location. Ignores the enemy if their location is the same as the given location
	UFUNCTION(BlueprintCallable) TArray<AEnemyCharacter*> GetEnemiesInRange(FVector location, float range);
	
	UFUNCTION(BlueprintCallable) void PauseWaveSpawning();
	UFUNCTION(BlueprintCallable) void ResumeWaveSpawning();

	//function called by the enemies notifying of their deactivation to keep track of active enemies
	void OnEnemyDeactivate();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ALevelDefinitionActor* GetLevelDef() const {return LevelDefinition;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCurrentWaveNbr() const { return CurrentWave; }

private:
	int CurrentWave = 1;
	int EnemiesLeftToSpawn;
	int EnemiesLeftInWave;
	int ActiveEnemyCount;
	float spawnDelay;
	FTimerHandle spawnTimer;
	FTimerHandle waveDelayTimer;
	FTimerHandle waveDelayTimer2;
	
	TArray<FEnemyTypeInfo> EnemyTypes;
	TArray<FEnemyTypeInfo> ActiveEnemyTypes;

	UPROPERTY(BlueprintAssignable, Category = "Level events")
	FWaveEvent OnLevelEnd;
	UPROPERTY(BlueprintAssignable, Category = "Level events")
	FWaveEvent OnWaveEnd;
	UPROPERTY(BlueprintAssignable, Category = "Level events")
	FWaveEvent OnWaveStart;
	
	UPROPERTY()
	TArray<ASpawnerActor*> SpawnPoints;
	UPROPERTY()
	TArray<AEnemyCharacter*> InstantiatedEnemies;

	UPROPERTY()
	TArray<AActor_Drop*> InstantiatedDrops;

	ALevelDefinitionActor* LevelDefinition;
	FVector EnemyStoreLocation = FVector::Zero();
	/**
	* Instantiates an Enemy of the given type and adds it to the list of Instantiated Enemies.
	* Should be used if no enemies of the needed type are available in the pool
	* @param EnemyBlueprint The type of enemy we want instantiated
	* @return returns a pointer to the enemy
	*/
	AEnemyCharacter* InstantiateEnemy(TSubclassOf<AEnemyCharacter> EnemyBlueprint);

	/**
	 * We instantiate a coin just like an enemy
	 */
	AActor_Drop* InstantiateCoinDrop();
	
	/**
	 * Starts the wave spawning logic. Sets the current wave to 0 and starts spawning random enemies at regular intervals
	 */
	void StartWaveSpawning();
	/**
	 * Advances the wave, checks if new enemy types need to be added to the list of enemies to spawn and sets them up
	 */
	void AdvanceToNextWave();
	
	/**
	 * Spawn Logic for a single enemy from the wave
	 */
	void WaveSpawn();
	void WaveSpawningStart();
	
	/**
	 * Gets called before a ULevel is removed from the world via UWorld::RemoveFromWorld or LoadMap, used to clean up all pointers to prevent stale ones which could cause a crash
	 */
	void OnLevelChange(ULevel* level, UWorld* LoadedWorld);

	void LevelEnd();
};
