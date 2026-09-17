// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Engine/StaticMesh.h"

#include "EnemyCharacter.generated.h"

class ASpawnerActor;
class ALevelDefinitionActor;

UCLASS(BlueprintType, Blueprintable)
class TPS_THISISMYDUNGEON_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Activates this enemy if not already activated and moves them to the specified location. If already activated, prints an error message
	 * @param instigator whoever called this function, used in the error message to say who caused the error
	 * @param Location the location the enemy should be moved to
	 * @param Rotation the rotation the enemy should have
	 */
	void Activate(ASpawnerActor* instigator, FVector Location, FRotator Rotation);
	/**
	 * Deactivates the enemy by stopping its rendering, disabling its AI if present and teleporting it to an inactive "store location"
	 */
	void Deactivate();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/**
	 * Heals the enemy by the given amount, not above max health
	 * @param amount amount of health to heal
	 * @return the "leftover". if the damage healed is less than the total heal amount
	 */
	virtual float Heal(float amount);
	virtual void OnDeath();
	
	bool IsActive() const { return bIsActive; }

	/**
	 * Called when the player reaches the objective, makes the player get the specified amount of treasure from the objective and updates the bool bHasTreasure, preventing them from taking more
	 * @param amount the amount of treausre (damage) to take(inflict)
	 */
	void AcquireTreasure(float amount);
	
	void SetMoveSpeed(float Amount);
	void SetMoveSpeedMult(float Mult);
	void ResetMoveSpeed();
	
	UBehaviorTree* GetBehaviorTree() const;
	ALevelDefinitionActor* GetLevelDefinition();
	float GetDamage() const { return Damage; }
	float GetTreasureDamage() const { return TreasureDamage; }
	/**
	 * Function that returns a bool specifying if the player is in sense range or not (if the player is too close, the enemy will target them over doing the objective)
	 * @return a bool specifying if the player is close enough or not 
	 */
	bool IsPlayerInSenseRange() const;
	bool CanMove() const;
	
	AActor* Target;
	TSubclassOf<UDamageType> DamageType;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsActive = false; 
	bool bIsDead = false;
	bool bCanAttack = true;
	bool bCanMove = true;
	bool bHasTreasure = false;

	float TreasureAmount{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	float MaxHealth{100.f};
	
	float Health{ 100.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	float Damage{ 10.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	float TreasureDamage{ 100.f };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	float MoveSpeed{ 600.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	float PlayerSenseRange{200.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true"))
	int Value{50};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	USoundBase* DeathCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta=(AllowPrivateAccess))
	TArray<USoundBase*> DamageTakenSoundCues;
	
	ALevelDefinitionActor* LevelDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent * ChestMesh;
	
};
