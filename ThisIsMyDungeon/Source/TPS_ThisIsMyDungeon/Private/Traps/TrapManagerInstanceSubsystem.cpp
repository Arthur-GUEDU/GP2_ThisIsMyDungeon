#include "Traps/TrapManagerInstanceSubsystem.h"

ABaseTrap* UTrapManagerInstanceSubsystem::GetInstantiatedTrapOfType(FTrapTypeInfo* TrapType)
{
	for (ABaseTrap* Trap : InstantiatedTraps)
	{
		if (!Trap->IsActive() && Trap->IsA(TrapType->TrapType))
		{
			UE_LOG(LogTemp, Warning, TEXT("Trap Manager: GetInstantiatedTrapOfType(): Returned Inactive Trap"));
			return Trap;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Trap Manager: GetInstantiatedTrapOfType(): Created New Trap"));
	return InstantiateTrap(TrapType->TrapType);
}

void UTrapManagerInstanceSubsystem::Spawn(FTrapTypeInfo* TrapType, FVector Location, FRotator Rotation)
{
	if (TrapType == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No trap type specified"));
		return;
	}

	if (ABaseTrap* Trap = GetInstantiatedTrapOfType(TrapType))
	{
		Trap->Spawn(Location, Rotation);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Trap Manager: Failed to spawn trap"));
}

void UTrapManagerInstanceSubsystem::Init(TArray<FTrapTypeInfo> TrapTypes)
{
	for (int i = 0; i < TrapTypes.Num(); ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			ABaseTrap * NewTrap = GetWorld()->SpawnActor<ABaseTrap>(TrapTypes[i].TrapType, TrapStoreLocation, FRotator::ZeroRotator);
			InstantiatedTraps.Add(NewTrap);
		}
	}
}

ABaseTrap* UTrapManagerInstanceSubsystem::InstantiateTrap(TSubclassOf<ABaseTrap> TrapBlueprint)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	ABaseTrap* NewTrap = GetWorld()->SpawnActor<ABaseTrap>(TrapBlueprint, TrapStoreLocation, FRotator::ZeroRotator, SpawnParams);
	InstantiatedTraps.Add(NewTrap);
	return NewTrap;
}
