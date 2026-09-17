#pragma once

#include "CoreMinimal.h"
#include "Traps/BaseTrap.h"
#include "Components/BoxComponent.h"
#include "BaseAttackTrap.generated.h"

/**
 * 
 */
UCLASS()
class TPS_THISISMYDUNGEON_API ABaseAttackTrap : public ABaseTrap
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Cooldown")
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Cooldown")
	float Timestamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Cooldown")
	class UTimer* Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damages;

	UBoxComponent* TriggerZone;

	virtual void BeginPlay() override;

	virtual void Attack();

	/// <summary>
	/// Try launching Attack if timer is finished and Enemies are in its TriggerZone
	/// </summary>
	UFUNCTION()
	void TryAttacking();

private:
	/// <summary>
	/// Method called when an Enemy enters its TriggerZone, adding it to the OverlapCount
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
	/// Method called when an Enemy leaves its TriggerZone, substracting it to the OverlapCount
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int OverlapCount = 0;
};
