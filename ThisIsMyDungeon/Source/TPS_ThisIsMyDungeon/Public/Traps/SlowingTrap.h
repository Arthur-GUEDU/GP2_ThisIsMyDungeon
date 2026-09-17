#pragma once

#include "CoreMinimal.h"
#include "Traps/BaseTrap.h"
#include "Components/BoxComponent.h"
#include "SlowingTrap.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API ASlowingTrap : public ABaseTrap
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASlowingTrap();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float SlowingRate;

	void BeginPlay(void) override;

private:
	UBoxComponent* TriggerZone;

	/// <summary>
	/// Method called when an Enemy enters its TriggerZone, slowing it down
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/// <summary>
	/// Method called when an Enemy leaves its TriggerZone, restoring its speed back up
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
