#pragma once

#include "CoreMinimal.h"
#include "Level/LevelDefinitionActor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TrapManagerInstanceSubsystem.generated.h"

/**
 *
 */
UCLASS()
class TPS_THISISMYDUNGEON_API UTrapManagerInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/// <summary>
	/// A function that returns a pointer to an instantiated trap of the specified type.
	/// If no instantiated trap of the specified type exists, a new one is created and added to the list
	/// </summary>
	/// <param name="TrapType">: The type of trap we want to get</param>
	/// <returns>A pointer to the instantiated trap</returns>
	ABaseTrap* GetInstantiatedTrapOfType(FTrapTypeInfo* TrapType);
	
	void Spawn(FTrapTypeInfo* TrapType, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void Init(TArray<FTrapTypeInfo> TrapTypes);
		
private:
	TArray<ABaseTrap*> InstantiatedTraps;

	FVector TrapStoreLocation = FVector::Zero();
	
	/// <summary>
	/// Instantiates a trap of the given type and adds it to the list of instantiated traps.
	/// Should be used if no traps of the needed type are available in the pool
	/// </summary>
	/// <param name="TrapBlueprint">: The type of trap we want instantiated</param>
	/// <returns>A pointer to the instanciated trap</returns>
	ABaseTrap* InstantiateTrap(TSubclassOf<ABaseTrap> TrapBlueprint);
};
