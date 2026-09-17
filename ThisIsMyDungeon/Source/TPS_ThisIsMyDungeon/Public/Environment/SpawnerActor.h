// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "SpawnerActor.generated.h"

class UEnemyManagerInstanceSubsystem;

UCLASS()
class TPS_THISISMYDUNGEON_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerActor();

	void Spawn(const TSubclassOf<AEnemyCharacter> EnemyType);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent* RootSceneComponent; //to let the Actor have a transform
	UEnemyManagerInstanceSubsystem* EnemyManagerSubSystem;

};
