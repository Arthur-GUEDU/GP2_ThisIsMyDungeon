#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

UCLASS()
class TPS_THISISMYDUNGEON_API ABaseTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bIsActive = true;

public:
	void Activate();
	void Deactivate();

	FORCEINLINE bool IsActive() { return bIsActive; };

	/// <summary>
	/// Spawn Trap at given location and rotated by a given rotation
	/// </summary>
	/// <param name="_Spawnpoint">: location to be spawned at</param>
	/// <param name="_Rotation">: rotation to be applied</param>
	void Spawn(FVector _Spawnpoint, FRotator _Rotation);
};
