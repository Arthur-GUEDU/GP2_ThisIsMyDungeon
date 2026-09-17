// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor_Drop.h"
#include "Enemies/EnemyCharacter.h"
#include "Traps/BaseTrap.h"
#include "Environment/ObjectiveActor.h"
#include "GameFramework/Actor.h"
#include "LevelDefinitionActor.generated.h"

USTRUCT(BlueprintType)
struct FEnemyTypeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyCharacter> EnemyType = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value = 0;

	//The Wave the enemy starts spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WaveStartSpawn = 0;
};

USTRUCT(BlueprintType)
struct FTrapTypeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseTrap> TrapType = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* TrapMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshOffset = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRotation = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshScale = FVector(1.0f, 1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TrapCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBlock = false;
};

/**
 * Level Definition, "Defines" everything that is needed for the level to function, like the Exit Location, the objective actor, spawn points, etc. Destroys itself and throws an error if not defined properly
 */
UCLASS()
class TPS_THISISMYDUNGEON_API ALevelDefinitionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelDefinitionActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WaveCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LevelStartDelay{60.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDelay{20.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* EnemyCountCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration{ 60.f };
	
	/**
 * Location where inactive enemies are stored and instanciated, if not specified automatically set to (0,0)
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget="true"))
	FVector EnemyStoreLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget="true"))
	FVector ExitLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyTypeInfo> EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxActiveEnemies = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AObjectiveActor* ObjectiveActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor_Drop> DropBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget="true"))
	TArray<FVector> SpawnPointsPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTrapTypeInfo> TrapTypes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
